cmake_minimum_required(VERSION 3.16)

add_library(events)
set_target_properties(events PROPERTIES LINKER_LANGUAGE CXX)

target_sources(events PUBLIC
        include/events/event.hpp
        include/events/eventdispatcher.hpp
        include/events/history.hpp)

target_include_directories(events INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include)
