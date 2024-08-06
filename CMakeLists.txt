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
        src/renderer/wiresRenderer.cpp
        src/renderer/wiresRenderer.h
        src/renderer/gridRenderer.cpp
        src/renderer/gridRenderer.h
        src/renderer/cursorRenderer.cpp
        src/renderer/cursorRenderer.h
        src/cursor.cpp
        src/cursor.h
        src/camera.cpp
        src/camera.h
        src/wires/wires.cpp
        src/wires/wires.h
        src/history/action.cpp
        src/history/action.h
        src/history/actions/createVertexAction.cpp
        src/history/actions/createVertexAction.h
        src/history/actions/insertVertexAction.cpp
        src/history/actions/insertVertexAction.h
        src/nodes/node.cpp
        src/nodes/node.h
        src/history/actions/moveVertexAction.cpp
        src/history/actions/moveVertexAction.h
        src/history/actions/createWireAction.cpp
        src/history/actions/createWireAction.h
        src/wires/networkResolver.cpp
        src/wires/networkResolver.h
        src/interaction.cpp
        src/interaction.h
        src/shapes/shapes.cpp
        src/shapes/shapes.h
        src/nodes/gate.cpp
        src/nodes/gate.h
        src/history/actions/moveGateAction.cpp
        src/history/actions/moveGateAction.h
        src/nodes/mesh.cpp
        src/nodes/mesh.h
)

target_link_libraries(${PROJECT_NAME} PRIVATE OpenGL)
target_link_libraries(${PROJECT_NAME} PRIVATE GLEW)
target_link_libraries(${PROJECT_NAME} PRIVATE glfw)

add_custom_target(copy_assets
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_LIST_DIR}/resources ${CMAKE_CURRENT_BINARY_DIR}/resources
)
add_dependencies(${PROJECT_NAME} copy_assets)
