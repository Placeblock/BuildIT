cmake_minimum_required(VERSION 3.16)

add_library(event STATIC
        src/event/event.cpp
        include/event/event.hpp
        src/event/history.cpp
        include/event/history.hpp
        src/event/eventtarget.cpp
        include/event/eventtarget.hpp
        include/event/ecs_history.hpp
        src/event/ecs_history.cpp)
set_target_properties(event PROPERTIES LINKER_LANGUAGE CXX)

target_include_directories(event PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(event PUBLIC EnTT::EnTT spdlog::spdlog ecs)

add_subdirectory(tests)
