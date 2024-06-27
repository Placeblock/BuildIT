cmake_minimum_required(VERSION 3.25)
project(buildit)

set(CMAKE_CXX_STANDARD 14)

add_executable(buildit
        src/gate.h
        src/gate.cpp
        src/buildit.cpp src/action.h src/selection.h src/selection.cpp)
