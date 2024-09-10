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
    std::set<Network*> updated;
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
    }

    GUI::Image::prerender(programs);
    if (this->dragging) {
        this->onDrag();
    }

    this->useFrameBuffer();
    programs->gridProgram->setVec2("resolution", this->getSize());
    programs->updateZoomUniforms(this->getSize(), this->camera);

    gridRenderer.render(programs->gridProgram);

    fontRenderer.render(programs->textureProgram);


    programs->vertexProgram->setFloat("size", 15.0);

    if (this->visualize) {
        this->visWiresRenderer.render(programs->wireProgram, programs->vertexProgram);
    }

    programs->vertexProgram->setFloat("size", 15.0);
}

CircuitBoard::CircuitBoard(GUI::View *view, uintVec2 size, Sim::Simulation* simulation)
    : simulation(simulation), simBridge(SimulationFeature(this->simulation, &this->nodes, &this->wires, &this->wiresRenderer)),
      selection(Selection{&this->simBridge, &this->wires, &this->wiresRenderer}),
      fontRenderer(FontRenderer(view->font)),
      FrameBufferRenderable(size),
      GUI::Image(view, size, this->frameTexture, false) {

}

void CircuitBoard::updateSize(uintVec2 newSize) {
    GUI::Image::updateSize(newSize);
    this->updateFrameBufferSize(newSize);
}