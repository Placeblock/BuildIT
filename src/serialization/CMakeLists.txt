cmake_minimum_required(VERSION 3.16)

add_library(serialization)
set_target_properties(serialization PROPERTIES LINKER_LANGUAGE CXX)

target_sources(serialization PRIVATE
        include/serialization/worldserialization.hpp
        src/serialization/worldserialization.cpp
    )

target_include_directories(serialization PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(serialization PRIVATE EnTT::EnTT bitsery)
