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
        src/graphics/circuitBoard/components/renderer/cablingRenderer.cpp
        src/graphics/circuitBoard/components/renderer/cablingRenderer.h
        src/graphics/circuitBoard/renderer/gridRenderer.cpp
        src/graphics/circuitBoard/renderer/gridRenderer.h
        src/graphics/circuitBoard/renderer/cursorRenderer.cpp
        src/graphics/circuitBoard/renderer/cursorRenderer.h
        src/graphics/circuitBoard/cursor.cpp
        src/graphics/circuitBoard/cursor.h
        src/graphics/data/camera.cpp
        src/graphics/data/camera.h
        src/graphics/circuitBoard/components/cabling/wire.cpp
        src/graphics/circuitBoard/components/cabling/wire.h
        src/graphics/circuitBoard/history/action.cpp
        src/graphics/circuitBoard/history/action.h
        src/graphics/circuitBoard/history/actions/insertJointAction.cpp
        src/graphics/circuitBoard/history/actions/insertJointAction.h
        src/graphics/circuitBoard/components/nodes/node.cpp
        src/graphics/circuitBoard/components/nodes/node.h
        src/graphics/circuitBoard/history/actions/moveComponentAction.cpp
        src/graphics/circuitBoard/history/actions/moveComponentAction.h
        src/graphics/circuitBoard/components/cabling/networkResolver.cpp
        src/graphics/circuitBoard/components/cabling/networkResolver.h
        src/graphics/shapes/shapes.cpp
        src/graphics/shapes/shapes.h
        src/graphics/circuitBoard/components/nodes/gate.cpp
        src/graphics/circuitBoard/components/nodes/gate.h
        src/graphics/renderer/instancedMeshRenderer.cpp
        src/graphics/renderer/instancedMeshRenderer.h
        src/simulation/simulation.cpp
        src/simulation/simulation.h
        src/simulation/node.cpp
        src/simulation/node.h
        src/simulation/updaters.cpp
        src/simulation/updaters.h
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
        src/graphics/circuitBoard/selection/selection.cpp
        src/graphics/circuitBoard/selection/selection.h
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
        src/graphics/circuitBoard/components/renderer/node/nodeRenderer.h
        src/graphics/circuitBoard/components/renderer/node/gateRenderer.h
        src/graphics/circuitBoard/components/renderer/node/notGateRenderer.h
        src/graphics/circuitBoard/components/renderer/node/gateRenderer.cpp
        src/graphics/circuitBoard/features/simulationFeature.cpp
        src/graphics/circuitBoard/features/simulationFeature.h
        src/graphics/circuitBoard/components/cabling/cabling.cpp
        src/graphics/circuitBoard/components/cabling/cabling.h
        src/graphics/circuitBoard/components/cabling/pin.h
        src/graphics/buffer/vertexBuffer.cpp
        src/graphics/buffer/vertexBuffer.h
        src/graphics/buffer/vertexArray.cpp
        src/graphics/buffer/vertexArray.h
        src/graphics/circuitBoard/observer.cpp
        src/graphics/circuitBoard/observer.h
        src/graphics/circuitBoard/features/navigationFeature.cpp
        src/graphics/circuitBoard/features/navigationFeature.h
        src/graphics/eventHandler.h
        src/graphics/circuitBoard/features/feature.cpp
        src/graphics/circuitBoard/features/feature.h
        src/graphics/circuitBoard/features/cursorFeature.cpp
        src/graphics/circuitBoard/features/cursorFeature.h
        src/graphics/circuitBoard/features/cablingFeature.cpp
        src/graphics/circuitBoard/features/cablingFeature.h
        src/graphics/circuitBoard/features/nodesFeature.cpp
        src/graphics/circuitBoard/features/nodesFeature.h
        src/graphics/circuitBoard/features/moveFeature.cpp
        src/graphics/circuitBoard/features/moveFeature.h
        src/graphics/circuitBoard/components/abstraction/movable.h
        src/graphics/circuitBoard/components/abstraction/rotatable.h
        src/graphics/circuitBoard/components/nodes/nodePins.cpp
        src/graphics/circuitBoard/components/nodes/nodePins.h
        src/graphics/circuitBoard/components/abstraction/movable.cpp
        src/graphics/circuitBoard/components/abstraction/rotatable.cpp
        src/graphics/circuitBoard/components/renderer/node/nodeRenderer.cpp
        src/graphics/circuitBoard/features/selectionFeature.cpp
        src/graphics/circuitBoard/features/selectionFeature.h
        src/graphics/circuitBoard/features/modifyCablingFeature.cpp
        src/graphics/circuitBoard/features/modifyCablingFeature.h
        src/graphics/circuitBoard/features/deleteFeature.cpp
        src/graphics/circuitBoard/features/deleteFeature.h
        src/graphics/circuitBoard/components/cabling/networkEvents.h
        src/graphics/circuitBoard/components/visitor.h
        src/graphics/circuitBoard/components/components.cpp
        src/graphics/circuitBoard/components/components.h
        src/graphics/circuitBoard/components/componentContainer.h
        src/graphics/circuitBoard/history/actions/createComponentAction.cpp
        src/graphics/circuitBoard/history/actions/createComponentAction.h
        src/graphics/circuitBoard/components/abstraction/boundingBox.h
        src/graphics/circuitBoard/components/collisionDetection.cpp
        src/graphics/circuitBoard/components/collisionDetection.h
        src/graphics/circuitBoard/components/renderer/componentRenderers.cpp
        src/graphics/circuitBoard/components/renderer/componentRenderers.h
        src/graphics/circuitBoard/components/renderer/componentRenderers.h
        src/graphics/circuitBoard/components/renderer/componentRenderers.h
        src/graphics/circuitBoard/components/nodes/notGate.cpp
        src/graphics/circuitBoard/components/nodes/notGate.h
        src/graphics/circuitBoard/features/historyFeature.cpp
        src/graphics/circuitBoard/features/historyFeature.h
        src/graphics/circuitBoard/features/updateFeature.cpp
        src/graphics/circuitBoard/features/updateFeature.h
        src/graphics/circuitBoard/events/componentSelectEvent.h
        src/graphics/circuitBoard/selection/selectable.cpp
        src/graphics/circuitBoard/selection/selectable.h
        src/graphics/circuitBoard/components/abstraction/interactable.h
        src/graphics/circuitBoard/components/abstraction/aABBInteractable.cpp
        src/graphics/circuitBoard/components/abstraction/aABBInteractable.h
        src/graphics/circuitBoard/components/abstraction/boundingBox.cpp
        src/graphics/circuitBoard/components/abstraction/component.cpp
        src/graphics/circuitBoard/components/abstraction/component.h
        src/graphics/circuitBoard/components/abstraction/circleInteractable.cpp
        src/graphics/circuitBoard/components/abstraction/circleInteractable.h
        src/graphics/circuitBoard/components/abstraction/lineInteractable.cpp
        src/graphics/circuitBoard/components/abstraction/lineInteractable.h
        src/graphics/circuitBoard/components/visitor.cpp
        src/graphics/circuitBoard/features/copyFeature.cpp
        src/graphics/circuitBoard/features/copyFeature.h
        src/graphics/circuitBoard/components/cabling/pin.cpp
        src/graphics/uniqueBlockingQueue.cpp
        src/graphics/uniqueBlockingQueue.h
        src/graphics/threadPool.cpp
        src/graphics/threadPool.h
        src/graphics/circuitBoard/events/historyChangeEvent.h
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
