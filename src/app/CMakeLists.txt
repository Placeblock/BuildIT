cmake_minimum_required(VERSION 3.16)

add_executable(app main.cpp
        ../history/include/history/worldhistory.hpp
        ../headless/main.cpp
        ../serialization/src/serialization/worldserialization.cpp
        ../serialization/include/serialization/worldserialization.hpp)
