cmake_minimum_required(VERSION 3.16)

add_library(plugin_default
        src/plugins/default/main.cpp)
set_target_properties(plugin_default PROPERTIES LINKER_LANGUAGE CXX)

target_sources(plugin_default PUBLIC)

target_include_directories(plugin_default PUBLIC include)
target_link_libraries(plugin_default PUBLIC model simulation plugin)

#add_subdirectory(tests)