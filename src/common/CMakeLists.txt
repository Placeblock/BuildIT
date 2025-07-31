cmake_minimum_required(VERSION 3.16)

add_library(common
        include/common/vector.hpp
        include/common/player.hpp)
set_target_properties(common PROPERTIES LINKER_LANGUAGE CXX)

target_include_directories(common PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
