//
// Created by felix on 8/2/24.
//

#include "action.h"

void Action::checkRegenerate(Wires *cables, WiresRenderer *renderer, bool regenerate) {
    if (regenerate) {
        renderer->regenerateData(&cables->vertices, &cables->wires);
    }
}
