add_library(math INTERFACE 
RectT.hpp
Vector2.hpp
Vector3.hpp
concepts.hpp
)

target_include_directories(math INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

