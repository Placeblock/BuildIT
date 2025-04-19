cmake_minimum_required(VERSION 3.16)

add_library(model INTERFACE)

target_link_libraries(model INTERFACE simulation)
target_include_directories(model INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include)
