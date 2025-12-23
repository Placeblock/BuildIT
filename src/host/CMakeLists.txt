cmake_minimum_required(VERSION 3.16)

add_library(host STATIC
        include/host/chip_type_registry.hpp
        src/chip_type_registry.cpp)
set_target_properties(host PROPERTIES LINKER_LANGUAGE CXX)

target_include_directories(host PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(host PUBLIC common simulation ecs EnTT::EnTT spdlog::spdlog glm::glm)