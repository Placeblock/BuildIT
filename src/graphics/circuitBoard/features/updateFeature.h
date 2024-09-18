//
// Created by felix on 9/14/24.
//

#ifndef BUILDIT_UPDATEFEATURE_H
#define BUILDIT_UPDATEFEATURE_H


#include "feature.h"
#include "graphics/circuitBoard/components/cabling/cabling.h"
#include "graphics/circuitBoard/components/componentContainer.h"

class UpdateFeature final : public Feature, public Updatable {
    Cabling *cabling;
    ComponentContainer *componentContainer;
public:
    UpdateFeature(Cabling *cabling, ComponentContainer *componentContainer);

    void update(float deltaTime) override;
};


#endif //BUILDIT_UPDATEFEATURE_H
