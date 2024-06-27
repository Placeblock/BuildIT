cmake_minimum_required(VERSION 3.25)
project(buildit)

set(CMAKE_CXX_STANDARD 20)

add_executable(buildit
        src/algorithm/algorithm.h
        src/algorithm/algorithm.cpp
        src/buildit.cpp src/action.h src/selection.h src/selection.cpp
        src/math/point.cpp
        src/math/point.h)
