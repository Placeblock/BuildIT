//
// Created by felix on 8/2/24.
//

#include "action.h"

void Action::execute(Action* action, const bool lastInBatch) {
    if (action->reversed) {
        action->rewind(lastInBatch);
    } else {
        action->execute(lastInBatch);
    }
}

void Action::rewind(Action* action, const bool lastInBatch) {
    if (action->reversed) {
        action->execute(lastInBatch);
    } else {
        action->rewind(lastInBatch);
    }
}
