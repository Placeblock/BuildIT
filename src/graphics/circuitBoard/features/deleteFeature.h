//
// Created by felix on 9/7/24.
//

#ifndef BUILDIT_DELETEFEATURE_H
#define BUILDIT_DELETEFEATURE_H


#include "feature.h"

class DeleteFeature : public Feature {
    void onKeyAction(glm::vec2 relPos, int key, int scanCode, int action, int mods) override;
};


#endif //BUILDIT_DELETEFEATURE_H
