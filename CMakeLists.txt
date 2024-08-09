cmake_minimum_required(VERSION 3.18)
project(buildit)

set(CMAKE_CXX_STANDARD 20)

include_directories(src include)

find_package(OpenGL REQUIRED)
find_package(GLEW 2.1.0 REQUIRED)
find_package(glfw3 REQUIRED)

add_executable(buildit
        src/buildit.cpp
        src/graphics/data/program.cpp
        src/graphics/data/program.h
        src/graphics/graphics.cpp
        src/graphics/graphics.h
        src/graphics/renderer/wiresRenderer.cpp
        src/graphics/renderer/wiresRenderer.h
        src/graphics/renderer/gridRenderer.cpp
        src/graphics/renderer/gridRenderer.h
        src/graphics/renderer/cursorRenderer.cpp
        src/graphics/renderer/cursorRenderer.h
        src/graphics/cursor.cpp
        src/graphics/cursor.h
        src/graphics/data/camera.cpp
        src/graphics/data/camera.h
        src/graphics/elements/wires/wires.cpp
        src/graphics/elements/wires/wires.h
        src/graphics/history/action.cpp
        src/graphics/history/action.h
        src/graphics/history/actions/createVertexAction.cpp
        src/graphics/history/actions/createVertexAction.h
        src/graphics/history/actions/insertVertexAction.cpp
        src/graphics/history/actions/insertVertexAction.h
        src/graphics/elements/nodes/node.cpp
        src/graphics/elements/nodes/node.h
        src/graphics/history/actions/moveVertexAction.cpp
        src/graphics/history/actions/moveVertexAction.h
        src/graphics/history/actions/createWireAction.cpp
        src/graphics/history/actions/createWireAction.h
        src/graphics/elements/wires/networkResolver.cpp
        src/graphics/elements/wires/networkResolver.h
        src/graphics/shapes/shapes.cpp
        src/graphics/shapes/shapes.h
        src/graphics/elements/nodes/gate.cpp
        src/graphics/elements/nodes/gate.h
        src/graphics/history/actions/moveGateAction.cpp
        src/graphics/history/actions/moveGateAction.h
        src/graphics/renderer/meshRenderer.cpp
        src/graphics/renderer/meshRenderer.h
        src/simulation/simulation.cpp
        src/simulation/simulation.h
        src/simulation/node.cpp
        src/simulation/node.h
        src/simulation/gate.cpp
        src/simulation/gate.h
        src/graphics/renderer/instancedVertexRenderer.cpp
        src/graphics/renderer/instancedVertexRenderer.h
        src/graphics/data/eventHandler.cpp
        src/graphics/data/eventHandler.h
        src/graphics/programs.cpp
        src/graphics/programs.h
        src/graphics/application.cpp
        src/graphics/application.h
        src/graphics/data/renderer.cpp
        src/graphics/data/renderer.h
        src/graphics/scene.cpp
        src/graphics/scene.h
        src/graphics/types.h
        src/graphics/data/selection.cpp
        src/graphics/data/selection.h
)

target_link_libraries(${PROJECT_NAME} PRIVATE OpenGL)
target_link_libraries(${PROJECT_NAME} PRIVATE GLEW)
target_link_libraries(${PROJECT_NAME} PRIVATE glfw)

add_custom_target(copy_assets
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_LIST_DIR}/resources ${CMAKE_CURRENT_BINARY_DIR}/resources
)
add_dependencies(${PROJECT_NAME} copy_assets)
