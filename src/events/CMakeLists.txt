cmake_minimum_required(VERSION 3.16)

add_library(events)
set_target_properties(events PROPERTIES LINKER_LANGUAGE CXX)

target_sources(events PUBLIC
        include/events.hpp
        src/events/handler/moveEventHandler.cpp
        )

target_include_directories(events PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(events PRIVATE model flecs::flecs_static)
