cmake_minimum_required(VERSION 3.16)

add_library(plugin
        src/plugin/plugin.cpp)
set_target_properties(plugin PROPERTIES LINKER_LANGUAGE CXX)

target_sources(plugin PUBLIC
        include/plugin/plugin.hpp)

target_include_directories(plugin PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(plugin PUBLIC ecs)