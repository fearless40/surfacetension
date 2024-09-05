

add_library(os INTERFACE os.hpp)
target_include_directories(os INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

add_subdirectory(windows)