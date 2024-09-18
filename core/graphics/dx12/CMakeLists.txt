add_library( dx12 
    dx12driver.cpp 
    dx12driver.hpp
    dx12helpers.hpp
)

target_include_directories( dx12 PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries( dx12 PRIVATE dxgi d3d12.lib directx-headers)


add_subdirectory(techniques)