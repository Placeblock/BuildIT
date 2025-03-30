qt_add_library(simulation STATIC
        node.h node.cpp
        simulation.cpp simulation.h
        graph.cpp graph.h
)
target_include_directories(simulation INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})