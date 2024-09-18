//
// Created by felix on 8/3/24.
//

#ifndef BUILDIT_MOVECOMPONENTACTION_H
#define BUILDIT_MOVECOMPONENTACTION_H

#include "graphics/circuitBoard/components/renderer/cablingRenderer.h"
#include "graphics/circuitBoard/history/action.h"

class MoveComponentAction : public Action {
private:
    Movable *movable;
    glm::vec2 delta;
public:
    MoveComponentAction(Movable *movable, glm::vec2 delta)
        : movable(movable), delta(delta), Action(false) {};
    void execute(bool lastInBatch) override;
    void rewind(bool lastInBatch) override;
};


#endif //BUILDIT_MOVECOMPONENTACTION_H
