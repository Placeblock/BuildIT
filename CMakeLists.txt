cmake_minimum_required(VERSION 3.28)
project(buildit)

set(CMAKE_CXX_STANDARD 20)

include_directories(src)
include_directories(src/graphics)
include_directories(src/math)
include_directories(src/simulation)

find_package(raylib 5.5.0 REQUIRED)

add_executable(buildit
        src/graphics/graphics.cpp
        src/graphics/graphics.h
        src/simulation/gate.cpp
        src/simulation/gate.h
        src/simulation/node.cpp
        src/simulation/node.h
        src/buildit.cpp
        src/graphics/node.cpp
        src/graphics/node.h
        src/graphics/gateNode.cpp
        src/graphics/gateNode.h
        src/simulation/simulation.cpp
        src/simulation/simulation.h
        src/graphics/gates/notGateNode.cpp
        src/graphics/gates/notGateNode.h
        src/graphics/gates/andGateNode.cpp
        src/graphics/gates/andGateNode.h
)

target_link_libraries(${PROJECT_NAME} PRIVATE raylib)

add_custom_target(copy_assets
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_LIST_DIR}/resources ${CMAKE_CURRENT_BINARY_DIR}/resources
)
add_dependencies(${PROJECT_NAME} copy_assets)