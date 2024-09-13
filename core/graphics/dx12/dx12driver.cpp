
#include "dx12driver.hpp"
#include "dx12helpers.hpp"
#include "../dxgi/dxgi.hpp"

void D3D12Driver::LoadPipeline(const LoadPiplineOptions &options)
{
    UINT dxgiFactoryFlags = 0;

#if defined(_DEBUG)
    // Enable the debug layer (requires the Graphics Tools "optional feature").
    // NOTE: Enabling the debug layer after device creation will invalidate the active device.
    {
        ComPtr<ID3D12Debug> debugController;
        if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController))))
        {
            debugController->EnableDebugLayer();

            // Enable additional debug layers.
            dxgiFactoryFlags |= DXGI_CREATE_FACTORY_DEBUG;
        }
    }
#endif

    ComPtr<IDXGIFactory4> factory;
    ThrowIfFailed(CreateDXGIFactory2(dxgiFactoryFlags, IID_PPV_ARGS(&factory)));

    if (options.use_warp_device)
    {
        ComPtr<IDXGIAdapter> warpAdapter;
        ThrowIfFailed(factory->EnumWarpAdapter(IID_PPV_ARGS(&warpAdapter)));

        ThrowIfFailed(D3D12CreateDevice(
            warpAdapter.Get(),
            D3D_FEATURE_LEVEL_11_0,
            IID_PPV_ARGS(&m_device)));
    }
    else
    {
        ComPtr<IDXGIAdapter1> hardwareAdapter;
        DXGI::GetHardwareAdapter(factory.Get(), &hardwareAdapter, true);

        ThrowIfFailed(D3D12CreateDevice(
            hardwareAdapter.Get(),
            D3D_FEATURE_LEVEL_11_0,
            IID_PPV_ARGS(&m_device)));
    }

    m_tearingSupport = DXGI::CheckTearingSupport();

    // Describe and create the command queue.
    D3D12_COMMAND_QUEUE_DESC queueDesc = {};
    queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
    queueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;

    ThrowIfFailed(m_device->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&m_commandQueue)));
    NAME_D3D12_OBJECT(m_commandQueue);

    // Describe and create the swap chain.
    // The resolution of the swap chain buffers will match the resolution of the window, enabling the
    // app to enter iFlip when in fullscreen mode. We will also keep a separate buffer that is not part
    // of the swap chain as an intermediate render target, whose resolution will control the rendering
    // resolution of the scene.
    DXGI_SWAP_CHAIN_DESC1 swapChainDesc = {};
    swapChainDesc.BufferCount = FrameCount;
    swapChainDesc.Width = options.width;
    swapChainDesc.Height = options.height;
    swapChainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
    swapChainDesc.SampleDesc.Count = 1;

    m_swapchain_resolution = {options.width, options.height};

    // It is recommended to always use the tearing flag when it is available.
    swapChainDesc.Flags = m_tearingSupport ? DXGI_SWAP_CHAIN_FLAG_ALLOW_TEARING : 0;

    ComPtr<IDXGISwapChain1> swapChain;
    ThrowIfFailed(factory->CreateSwapChainForHwnd(
        m_commandQueue.Get(), // Swap chain needs the queue so that it can force a flush on it.
        options.hwnd,
        &swapChainDesc,
        nullptr,
        nullptr,
        &swapChain));

    if (m_tearingSupport)
    {
        // When tearing support is enabled we will handle ALT+Enter key presses in the
        // window message loop rather than let DXGI handle it by calling SetFullscreenState.
        factory->MakeWindowAssociation(options.hwnd, DXGI_MWA_NO_ALT_ENTER);
    }

    ThrowIfFailed(swapChain.As(&m_swapChain));
    m_frameIndex = m_swapChain->GetCurrentBackBufferIndex();

    // Create descriptor heaps.
    {
        // Describe and create a render target view (RTV) descriptor heap.
        D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc = {};
        rtvHeapDesc.NumDescriptors = FrameCount + 1; // + 1 for the intermediate render target.
        rtvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
        rtvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
        ThrowIfFailed(m_device->CreateDescriptorHeap(&rtvHeapDesc, IID_PPV_ARGS(&m_rtvHeap)));

        // Describe and create a constant buffer view (CBV) and shader resource view (SRV) descriptor heap.
        D3D12_DESCRIPTOR_HEAP_DESC cbvSrvHeapDesc = {};
        cbvSrvHeapDesc.NumDescriptors = FrameCount + 1; // One CBV per frame and one SRV for the intermediate render target.
        cbvSrvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
        cbvSrvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
        ThrowIfFailed(m_device->CreateDescriptorHeap(&cbvSrvHeapDesc, IID_PPV_ARGS(&m_cbvSrvHeap)));

        m_rtvDescriptorSize = m_device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
        m_cbvSrvDescriptorSize = m_device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
    }

    // Create command allocators for each frame.
    for (UINT n = 0; n < FrameCount; n++)
    {
        ThrowIfFailed(m_device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&m_sceneCommandAllocators[n])));
        ThrowIfFailed(m_device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&m_postCommandAllocators[n])));
    }
}

void D3D12Driver::OnSizeChanged(unsigned int width, unsigned int height, bool minimized)
{
    // Determine if the swap buffers and other resources need to be resized or not.
    if ((width != m_swapchainResolution.width || height != m_swapchainResolution.height) && !minimized)
    {
        // Flush all current GPU commands.
        WaitForGpu();

        // Release the resources holding references to the swap chain (requirement of
        // IDXGISwapChain::ResizeBuffers) and reset the frame fence values to the
        // current fence value.
        for (UINT n = 0; n < FrameCount; n++)
        {
            m_renderTargets[n].Reset();
            m_fenceValues[n] = m_fenceValues[m_frameIndex];
        }

        // Resize the swap chain to the desired dimensions.
        DXGI_SWAP_CHAIN_DESC desc = {};
        m_swapChain->GetDesc(&desc);
        ThrowIfFailed(m_swapChain->ResizeBuffers(FrameCount, width, height, desc.BufferDesc.Format, desc.Flags));

        BOOL fullscreenState;
        ThrowIfFailed(m_swapChain->GetFullscreenState(&fullscreenState, nullptr));
        m_windowedMode = !fullscreenState;

        // Reset the frame index to the current back buffer index.
        m_frameIndex = m_swapChain->GetCurrentBackBufferIndex();

        // Update the width, height, and aspect ratio member variables.
        // UpdateForSizeChange(width, height);

        // LoadSizeDependentResources();
    }
}