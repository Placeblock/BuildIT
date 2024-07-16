cmake_minimum_required(VERSION 3.25)
project(buildit)

set(CMAKE_CXX_STANDARD 20)

add_executable(buildit
        src/buildit.cpp
        src/simulation/node.cpp
        src/simulation/node.h
        src/simulation/gate.cpp
        src/simulation/gate.h
)
