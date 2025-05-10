cmake_minimum_required(VERSION 3.16)

add_library(plugin_default)
set_target_properties(plugin_default PROPERTIES LINKER_LANGUAGE CXX)

target_sources(plugin_default PUBLIC
        include/plugins/default/components/orGate.hpp
        include/plugins/default/components/notGate.hpp
        include/plugins/default/components/andGate.hpp
        include/plugins/default.hpp
        src/plugins/default/components/orGate.cpp
        src/plugins/default/components/notGate.cpp
        src/plugins/default/components/andGate.cpp)

target_include_directories(plugin_default PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(plugin_default PUBLIC model simulation)

#add_subdirectory(tests)