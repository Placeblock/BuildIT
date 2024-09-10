//
// Created by felix on 8/3/24.
//

#ifndef BUILDIT_MOVECOMPONENTACTION_H
#define BUILDIT_MOVECOMPONENTACTION_H

#include "graphics/circuitBoard/components/renderer/cablingRenderer.h"
#include "graphics/circuitBoard/history/action.h"

class MoveComponentAction : public Action {
private:
    Movable *component;
    glm::vec2 newPos;
    glm::vec2 oldPos;
public:
    MoveComponentAction(Movable *component, glm::vec2 newPos)
        : component(component), newPos(newPos), Action(false) {};
    void execute(bool lastInBatch) override;
    void rewind(bool lastInBatch) override;
};


#endif //BUILDIT_MOVECOMPONENTACTION_H
