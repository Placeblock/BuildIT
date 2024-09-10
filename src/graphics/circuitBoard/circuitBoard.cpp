//
// Created by felix on 8/7/24.
//

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "circuitBoard.h"

#include <memory>
#include "graphics/circuitBoard/history/actions/moveComponentAction.h"
#include "graphics/circuitBoard/history/actions/createWireAction.h"
#include "graphics/circuitBoard/history/actions/insertJointAction.h"
#include "graphics/util.h"

void CircuitBoard::prerender(Programs* programs) {
    for (auto &updatable: this->updatableFeatures) {
        updatable->update(16.6); //TODO: REPLACE WITH REAL CALCULATED TIMESTEP
    }
    /*std::set<Network*> updated;
    for (const auto &node: this->nodes.joints) {
        if (node.second->resetUpdated()) {
            for (int i = 0; i < node.second->outputPins.size(); ++i) {
                const glm::vec2 outputPinCell = node.second->pos + glm::vec2(node.second->outputPins[i]);
                if (const auto joint = this->wires.getJoint(outputPinCell); joint != nullptr) {
                    if (updated.contains(joint->network)) continue;
                    updated.insert(joint->network);
                    joint->network->update();
                }
            }
        }
    }*/

    GUI::Image::prerender(programs);

    this->useFrameBuffer();
    programs->gridProgram->setVec2("resolution", this->getSize());
    programs->updateZoomUniforms(this->getSize(), this->camera);

    gridRenderer.render(programs->gridProgram);

    this->componentRenderers.render(programs);
    for (const auto &renderable: this->renderableFeatures) {
        renderable->render();
    }

    fontRenderer.render(programs->textureProgram);
}

CircuitBoard::CircuitBoard(GUI::View *view, uintVec2 size, Sim::Simulation* simulation)
    : simulation(simulation), fontRenderer(FontRenderer(view->font)), FrameBufferRenderable(size),
      GUI::Image(view, size, this->frameTexture, false) {
    this->updatableFeatures = {&cursorFeature};
    this->renderableFeatures = {&nodesFeature, &cablingFeature, &modifyCablingFeature, &moveFeature};
}

void CircuitBoard::updateSize(uintVec2 newSize) {
    GUI::Image::updateSize(newSize);
    this->updateFrameBufferSize(newSize);
}