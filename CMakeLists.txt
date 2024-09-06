cmake_minimum_required(VERSION 3.18)
project(buildit)

set(CMAKE_CXX_STANDARD 20)

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")

find_package(GLEW 2.1.0 REQUIRED)
find_package(glfw3 REQUIRED)
find_package(OpenGL REQUIRED)
#find_package(freetype2 REQUIRED)

add_executable(${PROJECT_NAME}
        src/buildit.cpp
        src/graphics/data/program.cpp
        src/graphics/data/program.h
        src/graphics/graphics.cpp
        src/graphics/graphics.h
        src/graphics/circuitBoard/renderer/wiresRenderer.cpp
        src/graphics/circuitBoard/renderer/wiresRenderer.h
        src/graphics/circuitBoard/renderer/gridRenderer.cpp
        src/graphics/circuitBoard/renderer/gridRenderer.h
        src/graphics/circuitBoard/renderer/cursorRenderer.cpp
        src/graphics/circuitBoard/renderer/cursorRenderer.h
        src/graphics/circuitBoard/cursor.cpp
        src/graphics/circuitBoard/cursor.h
        src/graphics/data/camera.cpp
        src/graphics/data/camera.h
        src/graphics/circuitBoard/components/wires/wire.cpp
        src/graphics/circuitBoard/components/wires/wire.h
        src/graphics/circuitBoard/history/action.cpp
        src/graphics/circuitBoard/history/action.h
        src/graphics/circuitBoard/history/actions/createJointAction.cpp
        src/graphics/circuitBoard/history/actions/createJointAction.h
        src/graphics/circuitBoard/history/actions/insertJointAction.cpp
        src/graphics/circuitBoard/history/actions/insertJointAction.h
        src/graphics/circuitBoard/components/nodes/node.cpp
        src/graphics/circuitBoard/components/nodes/node.h
        src/graphics/circuitBoard/history/actions/moveJointAction.cpp
        src/graphics/circuitBoard/history/actions/moveJointAction.h
        src/graphics/circuitBoard/history/actions/createWireAction.cpp
        src/graphics/circuitBoard/history/actions/createWireAction.h
        src/graphics/circuitBoard/components/wires/networkResolver.cpp
        src/graphics/circuitBoard/components/wires/networkResolver.h
        src/graphics/shapes/shapes.cpp
        src/graphics/shapes/shapes.h
        src/graphics/circuitBoard/components/nodes/gate.cpp
        src/graphics/circuitBoard/components/nodes/gate.h
        src/graphics/circuitBoard/history/actions/moveNodeAction.cpp
        src/graphics/circuitBoard/history/actions/moveNodeAction.h
        src/graphics/renderer/instancedMeshRenderer.cpp
        src/graphics/renderer/instancedMeshRenderer.h
        src/simulation/simulation.cpp
        src/simulation/simulation.h
        src/simulation/node.cpp
        src/simulation/node.h
        src/simulation/gate.cpp
        src/simulation/gate.h
        src/graphics/renderer/instancedVertexRenderer.cpp
        src/graphics/renderer/instancedVertexRenderer.h
        src/graphics/programs.cpp
        src/graphics/programs.h
        src/graphics/application.cpp
        src/graphics/application.h
        src/graphics/data/renderer.cpp
        src/graphics/data/renderer.h
        src/graphics/circuitBoard/circuitBoard.cpp
        src/graphics/circuitBoard/circuitBoard.h
        src/graphics/types.h
        src/graphics/circuitBoard/data/selection.cpp
        src/graphics/circuitBoard/data/selection.h
        src/graphics/circuitBoard/history/history.cpp
        src/graphics/circuitBoard/history/history.h
        src/graphics/circuitBoard/history/batchAction.cpp
        src/graphics/circuitBoard/history/batchAction.h
        src/graphics/kit/kit.cpp
        src/graphics/kit/kit.h
        src/graphics/data/frameBufferRenderable.cpp
        src/graphics/data/frameBufferRenderable.h
        src/graphics/font/fontDataLoader.cpp
        src/graphics/font/fontDataLoader.h
        src/graphics/font/fontLoader.cpp
        src/graphics/font/fontLoader.h
        src/graphics/font/fontMetrics.cpp
        src/graphics/font/fontMetrics.h
        src/graphics/font/fontRenderer.cpp
        src/graphics/font/fontRenderer.h
        src/graphics/kit/nodeList/nodeList.cpp
        src/graphics/kit/nodeList/nodeList.h
        src/graphics/gui/element.cpp
        src/graphics/gui/element.h
        src/graphics/gui/widgets/horizontalList.cpp
        src/graphics/gui/widgets/horizontalList.h
        src/graphics/gui/widgets/container.cpp
        src/graphics/gui/widgets/container.h
        src/graphics/gui/widgets/verticalList.cpp
        src/graphics/gui/widgets/verticalList.h
        src/graphics/gui/widgets/text.cpp
        src/graphics/gui/widgets/text.h
        src/graphics/gui/widgets/image.cpp
        src/graphics/gui/widgets/image.h
        src/graphics/kit/nodeList/nodeElement.h
        src/graphics/util.cpp
        src/graphics/util.h
        src/graphics/kit/nodeList/nodeDragHandler.h
        src/graphics/kit/nodeList/notNodeElement.cpp
        src/graphics/kit/nodeList/notNodeElement.h
        src/graphics/circuitBoard/components/nodes/renderer/nodeRenderer.h
        src/graphics/circuitBoard/components/nodes/renderer/gateRenderer.h
        src/graphics/circuitBoard/components/nodes/renderer/notNodeRenderer.h
        src/graphics/circuitBoard/components/nodes/renderer/nodeRenderers.h
        src/graphics/kit/nodeList/nodeAdder.h
        src/graphics/circuitBoard/history/actions/createNodeAction.cpp
        src/graphics/circuitBoard/history/actions/createNodeAction.h
        src/graphics/circuitBoard/components/nodes/renderer/gateRenderer.cpp
        src/graphics/circuitBoard/components/simulationBridge.cpp
        src/graphics/circuitBoard/components/simulationBridge.h
        src/graphics/circuitBoard/components/nodes/nodeContainer.h
        src/graphics/circuitBoard/components/nodes/nodes.cpp
        src/graphics/circuitBoard/components/nodes/nodes.h
        src/graphics/circuitBoard/components/wires/jointContainer.h
        src/graphics/circuitBoard/components/wires/wires.cpp
        src/graphics/circuitBoard/components/wires/wires.h
        src/graphics/circuitBoard/components/wires/wireContainer.h
        src/graphics/circuitBoard/components/wires/networkContainer.h
        src/graphics/circuitBoard/components/wires/pin.h
        src/graphics/buffer/vertexBuffer.cpp
        src/graphics/buffer/vertexBuffer.h
        src/graphics/buffer/vertexArray.cpp
        src/graphics/buffer/vertexArray.h
        src/graphics/circuitBoard/observer.cpp
        src/graphics/circuitBoard/observer.h
        src/graphics/circuitBoard/features/navigationFeature.cpp
        src/graphics/circuitBoard/features/navigationFeature.h
        src/graphics/eventHandler.cpp
        src/graphics/eventHandler.h
        src/graphics/circuitBoard/features/feature.cpp
        src/graphics/circuitBoard/features/feature.h
        src/graphics/circuitBoard/features/cursorFeature.cpp
        src/graphics/circuitBoard/features/cursorFeature.h
        src/graphics/circuitBoard/features/wiresFeature.cpp
        src/graphics/circuitBoard/features/wiresFeature.h
        src/graphics/circuitBoard/features/nodesFeature.cpp
        src/graphics/circuitBoard/features/nodesFeature.h
        src/graphics/circuitBoard/features/moveFeature.cpp
        src/graphics/circuitBoard/features/moveFeature.h
        src/graphics/circuitBoard/components/movable.h
        src/graphics/circuitBoard/components/component.cpp
        src/graphics/circuitBoard/components/component.h
        src/graphics/circuitBoard/components/positionable.h
        src/graphics/circuitBoard/components/rotatable.h
        src/graphics/circuitBoard/components/nodes/nodePins.cpp
        src/graphics/circuitBoard/components/nodes/nodePins.h
        src/graphics/circuitBoard/components/movable.cpp
        src/graphics/circuitBoard/components/positionable.cpp
        src/graphics/circuitBoard/components/rotatable.cpp
        src/graphics/circuitBoard/components/nodes/nodeContainer.cpp
        src/graphics/circuitBoard/components/nodes/nodeEvents.h
        src/graphics/circuitBoard/components/wires/jointEvents.h
        src/graphics/circuitBoard/components/wires/jointContainer.cpp
)

target_include_directories(${PROJECT_NAME} PRIVATE src include /usr/include/freetype2)

target_link_libraries(${PROJECT_NAME} PRIVATE OpenGL)
target_link_libraries(${PROJECT_NAME} PRIVATE GLEW)
target_link_libraries(${PROJECT_NAME} PRIVATE glfw)
#target_link_libraries(${PROJECT_NAME} PRIVATE freetype)

add_custom_target(copy_assets
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_LIST_DIR}/resources ${CMAKE_CURRENT_BINARY_DIR}/resources
)
add_dependencies(${PROJECT_NAME} copy_assets)
