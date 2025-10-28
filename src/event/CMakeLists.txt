cmake_minimum_required(VERSION 3.16)

add_library(event STATIC
        src/event/event_t.cpp
        include/event/event.hpp
        src/event/history.cpp
        include/event/history.hpp
        include/event/ecs_history.hpp
        src/event/ecs_history.cpp
        include/event/listener/create_chip_event.cpp
        include/event/listener/create_chip_event.h)
set_target_properties(event PROPERTIES LINKER_LANGUAGE CXX)

target_include_directories(event PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(event PUBLIC EnTT::EnTT spdlog::spdlog ecs glm::glm)