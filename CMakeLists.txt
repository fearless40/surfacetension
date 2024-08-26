cmake_minimum_required(VERSION 3.2...3.8)


### Require out-of-source builds
file(TO_CMAKE_PATH "${PROJECT_BINARY_DIR}/CMakeLists.txt" LOC_PATH)
if(EXISTS "${LOC_PATH}")
    message(FATAL_ERROR "You cannot build in a source directory (or any directory with a CMakeLists.txt file). Please make a build subdirectory. Feel free to remove CMakeCache.txt and CMakeFiles.")
endif()


project(Crossfire VERSION 1.0
        DESCRIPTION "Simple game using by dx12 graphics core"
        LANGUAGES CXX)

add_subdirectory(core)
add_subdirectory(apps)
add_subdirectory(extern)