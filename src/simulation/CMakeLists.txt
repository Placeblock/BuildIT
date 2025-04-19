cmake_minimum_required(VERSION 3.9)

add_library(simulation STATIC
        src/simulation/node.cpp
        src/simulation/simulation.cpp
        src/simulation/graph.cpp
)

target_include_directories(simulation PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

target_sources(simulation PRIVATE src/simulation/node.cpp src/simulation/simulation.cpp src/simulation/graph.cpp)

add_subdirectory(tests)