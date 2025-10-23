cmake_minimum_required(VERSION 3.16)

add_library(plugin_api_cpp STATIC
        src/buildit/plugin-api.cpp
)
set_target_properties(plugin_api_cpp PROPERTIES LINKER_LANGUAGE CXX)
target_link_libraries(plugin_api_cpp plugin_api)
target_include_directories(plugin_api_cpp PUBLIC include)


add_library(plugin_api_cpp_pic STATIC
        src/buildit/plugin-api.cpp
)
set_property(TARGET plugin_api_cpp_pic PROPERTY POSITION_INDEPENDENT_CODE ON)
set_target_properties(plugin_api_cpp_pic PROPERTIES LINKER_LANGUAGE CXX)
target_link_libraries(plugin_api_cpp_pic plugin_api_pic)
target_include_directories(plugin_api_cpp_pic PUBLIC include)