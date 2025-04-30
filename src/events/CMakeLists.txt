cmake_minimum_required(VERSION 3.16)

add_library(events)
set_target_properties(events PROPERTIES LINKER_LANGUAGE CXX)

target_sources(events PRIVATE
    include/events/history.hpp
    include/events/eventdispatcher.hpp
    include/events/event.hpp
    include/events/handler/eventHandler.hpp

    src/events/handler/eventHandler.cpp)

target_include_directories(events PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(events PRIVATE flecs::flecs_static simulation model)
