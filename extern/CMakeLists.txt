include(FetchContent)

FetchContent_Declare(
    D3D12MemoryAllocator 
    GIT_REPOSITORY https://github.com/GPUOpen-LibrariesAndSDKs/D3D12MemoryAllocator.git
    SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/D3D12MemoryAllocator"
    GIT_PROGRESS True
)

FetchContent_MakeAvailable(D3D12MemoryAllocator)

FetchContent_Declare(
    spdlog 
    GIT_REPOSITORY https://github.com/gabime/spdlog.git
    SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/spdlog"
    GIT_PROGRESS True
)

FetchContent_MakeAvailable(spdlog)

FetchContent_Declare(
    directx-headers 
    GIT_REPOSITORY https://github.com/microsoft/DirectX-Headers.git
    GIT_TAG origin/main
    SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/directx-headers"
    GIT_PROGRESS True
)

FetchContent_MakeAvailable(directx-headers)



add_subdirectory(DirectXTK12)

