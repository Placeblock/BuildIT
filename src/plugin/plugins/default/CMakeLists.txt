cmake_minimum_required(VERSION 3.16)

add_library(plugin_default SHARED
        src/plugins/default/main.cpp)
set_target_properties(plugin_default PROPERTIES LINKER_LANGUAGE CXX)

target_link_libraries(plugin_default PUBLIC plugin_api_cpp_pic)
target_link_options(plugin_default PRIVATE
        -static-libstdc++
        -static-libgcc
)