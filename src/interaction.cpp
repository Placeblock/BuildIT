//
// Created by felix on 8/3/24.
//

#include "interaction.h"
#include "wires/actions/moveVertexAction.h"

void Interaction::init() {
    this->visWiresRenderer.init();
}

void Interaction::regenerateVisData() {
    this->visWiresRenderer.regenerateData(&this->visVertices, &this->visWires);
}

void Interaction::renderVis(Shader* wireShader, Shader* vertexShader) {
    this->visWiresRenderer.render(wireShader, vertexShader);
}

Interaction::Interaction(Wires *wires, WiresRenderer *wiresRenderer) {
    this->wires = wires;
    this->wiresRenderer = wiresRenderer;
}

void Interaction::update(glm::vec2 mousePos, glm::vec2 cursorPos, bool shiftClick) {

}

void Interaction::handleLeftClick() {

}

void Interaction::handleRightClick() {

}

void Interaction::moveVertex(std::shared_ptr<Vertex> vertex, glm::vec2 newCell) {
    MoveVertexAction{vertex, newCell}.Execute(this->wires, this->wiresRenderer, false);
}
