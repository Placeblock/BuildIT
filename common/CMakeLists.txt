cmake_minimum_required(VERSION 4.0)
project(common C)

set(CMAKE_C_STANDARD 11)

include_directories(include)
include_directories(include/flecs)

add_executable(common
        include/flecs/flecs.h
        include/plugin.h
        src/flecs/flecs.c)
