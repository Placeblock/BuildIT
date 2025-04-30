cmake_minimum_required(VERSION 3.16)

add_library(controller
        src/controller/pinConnector.cpp
        include/controller/pinConnector.h)
set_target_properties(controller PROPERTIES LINKER_LANGUAGE CXX)

target_sources(controller PUBLIC
        include/controller.hpp)

target_include_directories(controller PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

target_link_libraries(controller PUBLIC simulation model flecs::flecs_static)
