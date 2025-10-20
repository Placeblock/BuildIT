cmake_minimum_required(VERSION 3.16)

add_library(plugin STATIC
        include/plugin/plugin-api.h
)
set_target_properties(plugin PROPERTIES LINKER_LANGUAGE CXX)

target_include_directories(plugin PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(plugin PUBLIC ecs)