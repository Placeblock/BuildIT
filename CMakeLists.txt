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
        src/renderer/linesRenderer.cpp
        src/renderer/linesRenderer.h
        src/renderer/gridRenderer.cpp
        src/renderer/gridRenderer.h
        src/renderer/cursorRenderer.cpp
        src/renderer/cursorRenderer.h
        src/cursor.cpp
        src/cursor.h
        src/camera.cpp
        src/camera.h
        src/cables/cables.cpp
        src/cables/cables.h
        src/util.h
        src/cables/action.cpp
        src/cables/action.h
        src/cables/actions/createVertexAction.cpp
        src/cables/actions/createVertexAction.h
        src/cables/actions/insertVertexAction.cpp
        src/cables/actions/insertVertexAction.h
)

target_link_libraries(${PROJECT_NAME} PRIVATE OpenGL)
target_link_libraries(${PROJECT_NAME} PRIVATE GLEW)
target_link_libraries(${PROJECT_NAME} PRIVATE glfw)

add_custom_target(copy_assets
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_LIST_DIR}/resources ${CMAKE_CURRENT_BINARY_DIR}/resources
)
add_dependencies(${PROJECT_NAME} copy_assets)
