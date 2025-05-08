cmake_minimum_required(VERSION 3.16)

add_library(history STATIC)
set_target_properties(history PROPERTIES LINKER_LANGUAGE CXX)

target_sources(history PRIVATE
        include/history/history.hpp
        src/history/history.cpp)

target_include_directories(history PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(history PRIVATE flecs::flecs_static spdlog::spdlog)

add_subdirectory(tests)