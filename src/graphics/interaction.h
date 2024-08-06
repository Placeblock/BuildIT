//
// Created by felix on 8/3/24.
//

#ifndef BUILDIT_INTERACTION_H
#define BUILDIT_INTERACTION_H


#include <set>
#include "elements/wires/wires.h"
#include "renderer/wiresRenderer.h"

class Interaction {
public:
    Interaction(Wires* wires, WiresRenderer* wiresRenderer);
    void init();
    void update(glm::vec2 mousePos, glm::vec2 cursorPos, bool shiftClick);
    void handleLeftClick();
    void handleRightClick();
    void regenerateVisData();
    void renderVis(Shader* wireShader, Shader* vertexShader);
private:
    Wires* wires;
    WiresRenderer* wiresRenderer;
    WiresRenderer visWiresRenderer;
    // We have to use shared_ptr because wires system needs it
    std::set<std::shared_ptr<Wire>> visWires;
    std::set<std::shared_ptr<Vertex>> visVertices;
    void moveVertex(std::shared_ptr<Vertex> vertex, glm::vec2 newCell);
};


#endif //BUILDIT_INTERACTION_H
