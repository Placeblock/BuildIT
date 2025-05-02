cmake_minimum_required(VERSION 3.16)

add_library(undo STATIC)
set_target_properties(undo PROPERTIES LINKER_LANGUAGE CXX)

target_sources(undo PRIVATE
        include/undo/undo.hpp)

target_include_directories(undo PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
