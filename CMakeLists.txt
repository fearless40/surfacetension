cmake_minimum_required(VERSION 3.2...3.8)


### Require out-of-source builds
file(TO_CMAKE_PATH "${PROJECT_BINARY_DIR}/CMakeLists.txt" LOC_PATH)
if(EXISTS "${LOC_PATH}")
    message(FATAL_ERROR "You cannot build in a source directory (or any directory with a CMakeLists.txt file). Please make a build subdirectory. Feel free to remove CMakeCache.txt and CMakeFiles.")
endif()


project(SurfaceTension VERSION 1.0
        DESCRIPTION "Simple game using by dx12 graphics core"
        LANGUAGES CXX)

set_property(GLOBAL PROPERTY CMAKE_CXX_STANDARD  20)
set_property(GLOBAL PROPERTY CMAKE_CXX_STANDARD_REQUIRED On)
set_property(GLOBAL PROPERTY CMAKE_CXX_EXTENSIONS Off)

if (MSVC_VERSION GREATER_EQUAL "1900")
    include(CheckCXXCompilerFlag)
    CHECK_CXX_COMPILER_FLAG("/std:c++20" _cpp_20_flag_supported)
    if (_cpp_20_flag_supported)
        add_compile_options("/std:c++20")
    endif()
endif()

add_subdirectory(core)
add_subdirectory(apps)
add_subdirectory(extern)