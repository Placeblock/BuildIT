cmake_minimum_required(VERSION 3.16)

add_library(plugin_api STATIC
        include/plugin-api/plugin-api.h
)
set_target_properties(plugin_api PROPERTIES LINKER_LANGUAGE CXX)

target_include_directories(plugin_api PUBLIC include)