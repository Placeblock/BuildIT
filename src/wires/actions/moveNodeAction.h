//
// Created by felix on 8/3/24.
//

#ifndef BUILDIT_MOVENODEACTION_H
#define BUILDIT_MOVENODEACTION_H


#include "wires/action.h"

class MoveNodeAction : public Action {
private:
    std::shared_ptr<Node> node;
    glm::vec2 newCell;
    glm::vec2 oldCell;
public:
    explicit MoveNodeAction(std::shared_ptr<Node> node, glm::vec2 newCell);
    void Execute(Wires* cables) override;
    void Rewind(Wires* cables) override;
};


#endif //BUILDIT_MOVENODEACTION_H
