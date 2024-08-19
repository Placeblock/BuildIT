//
// Created by felix on 8/9/24.
//

#ifndef BUILDIT_SELECTION_H
#define BUILDIT_SELECTION_H


#include <set>
#include <memory>
#include "graphics/circuitBoard/elements/wires/wires.h"
#include "graphics/circuitBoard/renderer/wiresRenderer.h"

const glm::vec3 selectedVertexColor{200, 100, 0};

class Selection {
private:
    Wires* wires;
    WiresRenderer* renderer;
public:
    Selection(Wires* wires, WiresRenderer* renderer) : wires(wires), renderer(renderer) {};
    std::set<const Vertex*> vertices;
    void clear();
    void addVertex(const Vertex* vertex);
    void removeVertex(const Vertex* vertex, bool erase);

};


#endif //BUILDIT_SELECTION_H
