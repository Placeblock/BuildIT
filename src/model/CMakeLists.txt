cmake_minimum_required(VERSION 3.16)

add_library(model)
set_target_properties(model PROPERTIES LINKER_LANGUAGE CXX)

target_sources(model PUBLIC
        include/model.hpp
        include/model/components.hpp
        include/model/network.hpp)

target_link_libraries(model INTERFACE simulation)

add_subdirectory(tests)