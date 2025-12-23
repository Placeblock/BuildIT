cmake_minimum_required(VERSION 3.16)

add_library(serialization STATIC
        include/serialization/serialization.hpp
        src/serialization/serialization.cpp)
set_target_properties(serialization PROPERTIES LINKER_LANGUAGE CXX)

target_include_directories(serialization PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(serialization PUBLIC ecs EnTT::EnTT)
