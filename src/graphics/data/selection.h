//
// Created by felix on 8/9/24.
//

#ifndef BUILDIT_SELECTION_H
#define BUILDIT_SELECTION_H


#include <set>
#include <memory>
#include "graphics/elements/wires/wires.h"
#include "graphics/renderer/wiresRenderer.h"

const glm::vec3 selectedVertexColor{200, 100, 0};

class Selection {
private:
    Wires* wires;
    WiresRenderer* renderer;
public:
    Selection(Wires* wires, WiresRenderer* renderer) : wires(wires), renderer(renderer) {};
    std::set<std::shared_ptr<Vertex>> vertices;
    void clear();
    void addVertex(const std::shared_ptr<Vertex>& vertex);
    void removeVertex(const std::shared_ptr<Vertex>& vertex);

};


#endif //BUILDIT_SELECTION_H
