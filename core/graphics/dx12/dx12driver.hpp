#pragma once

#include <dxgi1_6.h>
#include <d3d12.h>
#include <wrl.h>

using namespace DirectX;

// Note that while ComPtr is used to manage the lifetime of resources on the CPU,
// it has no understanding of the lifetime of resources on the GPU. Apps must account
// for the GPU lifetime of resources to avoid destroying objects that may still be
// referenced by the GPU.
// An example of this can be found in the class method: OnDestroy().
using Microsoft::WRL::ComPtr;

using uint = unsigned int; 
using uint64 = unsigned long; 

// Illustrate how to handle window resizing and fullscreen mode.
class D3D12Driver
{
public:
    struct Resolution
    {
        unsigned int width;
        unsigned int height;
    };

    struct SwapChainSyncronization
    {
      uint64 fenceValue; 
    };

    static const uint SwapChainCount{2};

protected:
    // Swap chain details
    unsigned int m_swapchainFrameCount{SwapChainCount};
    Resolution m_swapchainResolution;
    Resolution m_intermediateRenderTargetResolution;

    // Synchronization objects.
    UINT m_frameIndex;
    HANDLE m_fenceEvent;
    ComPtr<ID3D12Fence> m_fence;
    UINT64 m_fenceValues[SwapChainCount];

    SwapChainSyncronization m_swapchainSync; 

    // Pipeline objects.
    /*CD3DX12_VIEWPORT m_sceneViewport;
    CD3DX12_VIEWPORT m_postViewport;
    CD3DX12_RECT m_sceneScissorRect;
    CD3DX12_RECT m_postScissorRect;*/
    ComPtr<IDXGISwapChain3> m_swapChain;
    ComPtr<ID3D12Device> m_device;
    ComPtr<ID3D12Resource> m_renderTargets[SwapChainCount];
    ComPtr<ID3D12Resource> m_intermediateRenderTarget;
    ComPtr<ID3D12CommandAllocator> m_sceneCommandAllocators[SwapChainCount];
    ComPtr<ID3D12CommandAllocator> m_postCommandAllocators[SwapChainCount];
    ComPtr<ID3D12CommandQueue> m_commandQueue;
    ComPtr<ID3D12RootSignature> m_sceneRootSignature;
    ComPtr<ID3D12RootSignature> m_postRootSignature;
    ComPtr<ID3D12DescriptorHeap> m_rtvHeap;
    ComPtr<ID3D12DescriptorHeap> m_cbvSrvHeap;
    ComPtr<ID3D12PipelineState> m_scenePipelineState;
    ComPtr<ID3D12PipelineState> m_postPipelineState;
    ComPtr<ID3D12GraphicsCommandList> m_sceneCommandList;
    ComPtr<ID3D12GraphicsCommandList> m_postCommandList;
    
    
    UINT m_rtvDescriptorSize;
    UINT m_cbvSrvDescriptorSize;

public:
    struct LoadPiplineOptions
    {
        unsigned int width;
        unsigned int height;
        unsigned int swapchain_frame_count{2};
        HWND hwnd;
        bool use_warp_device{false};
    };

    uint get_rtv_descriptor_size() { return m_rtvDescriptorSize; }
    uint get_cbv_srv_descriptor_size() { return m_cbvSrvDescriptorSize; }

    IDXGISwapChain *get_swapchain() { return m_swapChain.Get(); }
    ID3D12Device *get_device() { return m_device.Get(); }
    
    void WaitForGpu();
    
protected:
    virtual void OnInit();
    virtual void OnUpdate();
    virtual void OnRender();
    virtual void OnSizeChanged(UINT width, UINT height, bool minimized);
    virtual void OnWindowMoved(int, int);
    virtual void OnDestroy();
    virtual void OnKeyDown(UINT8 key);

private:
    static const UINT FrameCount = 2;
    static const float QuadWidth;
    static const float QuadHeight;
    static const float LetterboxColor[4];
    static const float ClearColor[4];

    struct Resolution
    {
        UINT Width;
        UINT Height;
    };

    static const Resolution m_resolutionOptions[];
    static const UINT m_resolutionOptionsCount;
    static UINT m_resolutionIndex; // Index of the current scene rendering resolution from m_resolutionOptions.

    // App resources.
    ComPtr<ID3D12Resource> m_sceneVertexBuffer;
    D3D12_VERTEX_BUFFER_VIEW m_sceneVertexBufferView;
    ComPtr<ID3D12Resource> m_postVertexBuffer;
    D3D12_VERTEX_BUFFER_VIEW m_postVertexBufferView;
    ComPtr<ID3D12Resource> m_sceneConstantBuffer;
    SceneConstantBuffer m_sceneConstantBufferData;
    UINT8 *m_pCbvDataBegin;

    // Synchronization objects.
    UINT m_frameIndex;
    HANDLE m_fenceEvent;
    ComPtr<ID3D12Fence> m_fence;
    UINT64 m_fenceValues[FrameCount];

    // Track the state of the window.
    // If it's minimized the app may decide not to render frames.
    bool m_windowVisible;
    bool m_windowedMode;
    bool m_tearingSupport;

    void LoadPipeline(const LoadPiplineOptions &options);
    void LoadAssets();
    void RestoreD3DResources();
    void ReleaseD3DResources();
    void LoadSizeDependentResources();
    void LoadSceneResolutionDependentResources();
    void PopulateCommandLists();
    void WaitForGpu();
    void MoveToNextFrame();
    void UpdatePostViewAndScissor();
    void UpdateTitle();
};