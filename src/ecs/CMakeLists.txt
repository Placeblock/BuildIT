cmake_minimum_required(VERSION 3.16)

add_library(ecs STATIC
        include/ecs/collision.hpp
        include/ecs/components.hpp
        src/ecs/components.cpp
        src/ecs/collision.cpp
        include/ecs/internal/globalentitymixin.hpp
        include/ecs/internal/changemixin.hpp
        include/ecs/internal/snapshot.hpp
        src/ecs/internal/snapshot.cpp
        include/ecs/circuitboard.h
        src/ecs/circuitboard.cpp
        include/ecs/chip_type_registry.hpp
        src/ecs/chip_type_registry.cpp)
set_target_properties(ecs PROPERTIES LINKER_LANGUAGE CXX)

target_include_directories(ecs PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(ecs PUBLIC common simulation EnTT::EnTT spdlog::spdlog Bitsery::bitsery glm::glm)