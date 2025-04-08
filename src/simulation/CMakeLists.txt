add_library(simulation STATIC
        node.hpp node.cpp
        simulation.cpp simulation.hpp
        graph.cpp graph.hpp
)
target_include_directories(simulation INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})