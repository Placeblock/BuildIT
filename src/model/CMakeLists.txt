cmake_minimum_required(VERSION 3.16)

add_library(model)
set_target_properties(model PROPERTIES LINKER_LANGUAGE CXX)

target_sources(model PRIVATE
        include/model/collision.hpp
        include/model/components.hpp
        include/model/pinRenderCache.hpp
        src/model/components.cpp
        src/model/pinRenderCache.cpp
        src/model/collision.cpp)

target_include_directories(model PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(model PRIVATE simulation flecs::flecs_static)

add_subdirectory(tests)
