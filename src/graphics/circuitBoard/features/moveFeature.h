//
// Created by felix on 8/31/24.
//

#ifndef BUILDIT_MOVEFEATURE_H
#define BUILDIT_MOVEFEATURE_H


#include "feature.h"

/**
 * Adds functionality for moving things around
 */
class MoveFeature : public Feature, public Updatable {
public:
    void onMouseAction(glm::vec2 relPos, int button, int action, int mods) override;
    void onMouseMove(glm::vec2 relPos, glm::vec2 delta) override;
};


#endif //BUILDIT_MOVEFEATURE_H
