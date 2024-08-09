//
// Created by felix on 8/3/24.
//

#ifndef BUILDIT_MOVEVERTEXACTION_H
#define BUILDIT_MOVEVERTEXACTION_H


#include <utility>

#include "graphics/history/wiresAction.h"

class MoveVertexAction : public WiresAction {
private:
    std::shared_ptr<Vertex> vertex;
    glm::vec2 newCell;
    glm::vec2 oldCell;
    void updateCellData(Wires* wires, WiresRenderer* renderer);
public:
    MoveVertexAction(std::shared_ptr<Vertex> node, glm::vec2 newCell, Wires* wires, WiresRenderer* renderer, bool regenerate, bool reversed)
        : vertex(std::move(node)), newCell(newCell), WiresAction(wires, renderer, regenerate, reversed) {};
    void execute() override;
    void rewind() override;
};


#endif //BUILDIT_MOVEVERTEXACTION_H
