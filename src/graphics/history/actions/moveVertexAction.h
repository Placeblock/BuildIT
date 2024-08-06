//
// Created by felix on 8/3/24.
//

#ifndef BUILDIT_MOVEVERTEXACTION_H
#define BUILDIT_MOVEVERTEXACTION_H


#include "graphics/history/action.h"

class MoveVertexAction : public Action {
private:
    std::shared_ptr<Vertex> vertex;
    glm::vec2 newCell;
    glm::vec2 oldCell;
    void updateCellData(Wires* wires, WiresRenderer* renderer);
public:
    explicit MoveVertexAction(std::shared_ptr<Vertex> node, glm::vec2 newCell);
    void Execute(Wires* wires, WiresRenderer* renderer, bool regenerate) override;
    void Rewind(Wires* wires, WiresRenderer* renderer, bool regenerate) override;
};


#endif //BUILDIT_MOVEVERTEXACTION_H
