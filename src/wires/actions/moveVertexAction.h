//
// Created by felix on 8/3/24.
//

#ifndef BUILDIT_MOVEVERTEXACTION_H
#define BUILDIT_MOVEVERTEXACTION_H


#include "wires/action.h"

class MoveVertexAction : public Action {
private:
    std::shared_ptr<Vertex> vertex;
    glm::vec2 newCell;
    glm::vec2 oldCell;
public:
    explicit MoveVertexAction(std::shared_ptr<Vertex> node, glm::vec2 newCell);
    void Execute(Wires* cables) override;
    void Rewind(Wires* cables) override;
};


#endif //BUILDIT_MOVEVERTEXACTION_H
