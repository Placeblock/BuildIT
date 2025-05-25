cmake_minimum_required(VERSION 3.16)

add_library(simulation STATIC)
set_target_properties(simulation PROPERTIES LINKER_LANGUAGE CXX)

target_sources(simulation PRIVATE
        include/simulation/node.hpp
        include/simulation/simulation.hpp
        include/simulation/graph.hpp
        src/simulation/node.cpp
        src/simulation/simulation.cpp
        src/simulation/graph.cpp)

target_include_directories(simulation PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

add_subdirectory(tests)
