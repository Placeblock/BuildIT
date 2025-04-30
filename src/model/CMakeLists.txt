cmake_minimum_required(VERSION 3.16)

add_library(model)
set_target_properties(model PROPERTIES LINKER_LANGUAGE CXX)

target_sources(model PUBLIC
        include/model.hpp
        src/model/components.cpp
        src/model/pinRenderCache.cpp)

target_include_directories(model PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(model PUBLIC simulation flecs::flecs_static)

add_subdirectory(tests)