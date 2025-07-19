cmake_minimum_required(VERSION 3.16)

add_library(ecs)
set_target_properties(ecs PROPERTIES LINKER_LANGUAGE CXX)

target_sources(ecs PRIVATE
        include/ecs/collision.hpp
        include/ecs/components.hpp
        src/ecs/components.cpp
        src/ecs/collision.cpp
        include/ecs/globalentitymixin.hpp
        include/ecs/changemixin.hpp
        include/ecs/snapshot.hpp
        src/ecs/snapshot.cpp
        include/ecs/circuitboard.h
        src/ecs/circuitboard.cpp
        include/ecs/component_registry.hpp
        src/ecs/component_registry.cpp)

target_include_directories(ecs PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(ecs PUBLIC common simulation EnTT::EnTT spdlog::spdlog Bitsery::bitsery)

add_subdirectory(tests)
