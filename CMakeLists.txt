cmake_minimum_required(VERSION 3.28)
project(buildit)

set(CMAKE_CXX_STANDARD 20)

include_directories(src include)

find_package(OpenGL REQUIRED)
find_package(GLEW 2.2.0 REQUIRED)
find_package(glfw3 REQUIRED)

add_executable(buildit
        src/buildit.cpp
        src/shader.cpp
        src/shader.h
        src/graphics.cpp
        src/graphics.h
        src/Lines.cpp
        src/Lines.h
        src/grid.cpp
        src/grid.h
        src/cursor.cpp
        src/cursor.h
)

target_link_libraries(${PROJECT_NAME} PRIVATE OpenGL)
target_link_libraries(${PROJECT_NAME} PRIVATE GLEW)
target_link_libraries(${PROJECT_NAME} PRIVATE glfw)

add_custom_target(copy_assets
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_LIST_DIR}/resources ${CMAKE_CURRENT_BINARY_DIR}/resources
)
add_dependencies(${PROJECT_NAME} copy_assets)
