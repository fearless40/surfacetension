cmake_minimum_required(VERSION 3.2...3.8)

project(crossfireapp)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)


add_executable(crossfireapp )
target_sources(crossfireapp PRIVATE crossfire.cpp crossfire.h game.cpp game.h main.cpp)
target_link_libraries(crossfireapp core)
