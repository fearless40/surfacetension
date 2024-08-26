include(FetchContent)

FetchContent_Declare(
    D3D12MemoryAllocator 
    GIT_REPOSITORY https://github.com/GPUOpen-LibrariesAndSDKs/D3D12MemoryAllocator.git
    SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/D3D12MemoryAllocator"
    GIT_PROGRESS True
)

FetchContent_MakeAvailable(D3D12MemoryAllocator)

