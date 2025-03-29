qt_add_library(simulation STATIC
        Node.h Node.cpp
        Simulation.cpp Simulation.h
        Graph.cpp Graph.h
)
target_include_directories(simulation INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})