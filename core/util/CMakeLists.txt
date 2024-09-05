add_library(util INTERFACE 
uniqueint.hpp
soa.hpp
overload.hpp
event.hpp
TimeStep.hpp
)
target_include_directories(util INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
add_library(surfacetension::util ALIAS util)
