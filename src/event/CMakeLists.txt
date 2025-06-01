cmake_minimum_required(VERSION 3.16)

add_library(event STATIC)
set_target_properties(event PROPERTIES LINKER_LANGUAGE CXX)

target_sources(event PRIVATE
        src/event/event.cpp
        include/event/event.hpp
        src/event/history.cpp
        include/event/history.hpp
        include/event/eventdispatcher.hpp
        src/event/eventtarget.cpp
        include/event/eventtarget.hpp
        include/event/ecs_history.hpp
        src/event/ecs_history.cpp)

target_include_directories(event PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(event PRIVATE EnTT::EnTT spdlog::spdlog model)

add_subdirectory(tests)
