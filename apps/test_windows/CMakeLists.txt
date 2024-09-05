add_executable(test_window WIN32 main.cpp)
target_link_libraries(test_window PUBLIC 
    os
    windows
    PRIVATE spdlog
)