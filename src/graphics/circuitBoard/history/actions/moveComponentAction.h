//
// Created by felix on 8/3/24.
//

#ifndef BUILDIT_MOVECOMPONENTACTION_H
#define BUILDIT_MOVECOMPONENTACTION_H

#include "graphics/circuitBoard/components/renderer/cablingRenderer.h"
#include "graphics/circuitBoard/history/action.h"

class MoveComponentAction final : public Action {
    Movable *movable;
    glm::vec2 delta;
public:
    MoveComponentAction(Movable *movable, const glm::vec2 delta)
        : Action(false), movable(movable), delta(delta) {};
    void execute(bool lastInBatch) override;
    void rewind(bool lastInBatch) override;
};


#endif //BUILDIT_MOVECOMPONENTACTION_H
