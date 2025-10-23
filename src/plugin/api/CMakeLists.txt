cmake_minimum_required(VERSION 3.16)

add_library(plugin_api STATIC
        include/buildit/plugin-api.h
)
set_target_properties(plugin_api PROPERTIES LINKER_LANGUAGE CXX)
target_include_directories(plugin_api PUBLIC include)

add_library(plugin_api_pic STATIC
        include/buildit/plugin-api.h
)
set_property(TARGET plugin_api_pic PROPERTY POSITION_INDEPENDENT_CODE ON)
set_target_properties(plugin_api_pic PROPERTIES LINKER_LANGUAGE CXX)
target_include_directories(plugin_api_pic PUBLIC include)