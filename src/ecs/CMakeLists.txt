cmake_minimum_required(VERSION 3.16)

add_library(ecs STATIC
        include/ecs/registry_node.hpp)
set_target_properties(ecs PROPERTIES LINKER_LANGUAGE CXX)
target_include_directories(ecs PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(ecs PUBLIC common simulation spdlog::spdlog)