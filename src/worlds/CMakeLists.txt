cmake_minimum_required(VERSION 3.16)

add_library(worlds)
set_target_properties(worlds PROPERTIES LINKER_LANGUAGE CXX)

target_sources(worlds PUBLIC
        include/worlds.hpp
        src/worlds/worldRegistry.cpp)

target_include_directories(worlds PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(worlds PUBLIC model flecs::flecs_static)