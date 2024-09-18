add_library( dx12 
    dx12driver.cpp 
    dx12driver.hpp
    dx12helpers.hpp
)

target_include_directories( dx12 PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries( dx12 
    PRIVATE 
        d3d12.lib dxgi.lib dxguid.lib
        dxgi 
        DirectX-Headers windows
)
target_precompile_headers(dx12 REUSE_FROM windows)


add_subdirectory(techniques)