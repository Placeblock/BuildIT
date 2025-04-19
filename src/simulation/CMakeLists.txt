cmake_minimum_required(VERSION 3.9)

add_library(simulation STATIC
        src/node.cpp
        src/simulation.cpp
        src/graph.cpp
)

target_include_directories(simulation PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

target_sources(simulation PRIVATE src/node.cpp src/simulation.cpp src/graph.cpp)

add_subdirectory(tests)