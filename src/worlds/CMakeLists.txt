cmake_minimum_required(VERSION 3.16)

add_library(worlds)
set_target_properties(worlds PROPERTIES LINKER_LANGUAGE CXX)

target_sources(worlds PRIVATE
        src/worlds/world.cpp
        include/worlds/world.hpp)

target_include_directories(worlds PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(worlds PRIVATE model EnTT::EnTT)
