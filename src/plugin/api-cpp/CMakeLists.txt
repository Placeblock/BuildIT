cmake_minimum_required(VERSION 3.16)

add_library(plugin_api_cpp STATIC
        src/buildit/plugin-api.cpp
)
set_target_properties(plugin_api_cpp PROPERTIES LINKER_LANGUAGE CXX)
target_link_libraries(plugin_api_cpp plugin_api)
target_include_directories(plugin_api_cpp PUBLIC include)