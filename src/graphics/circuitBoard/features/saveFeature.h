//
// Created by felix on 9/27/24.
//

#ifndef BUILDIT_SAVEFEATURE_H
#define BUILDIT_SAVEFEATURE_H

#include "feature.h"
#include "simulation/simulation.h"

class ComponentContainer;

class SaveFeature : public Feature {
    ComponentContainer *componentContainer;
    Sim::Simulation *simulation;
public:
    SaveFeature(ComponentContainer *componentContainer, Sim::Simulation *simulation);

    void onKeyAction(glm::vec2 relPos, int key, int scanCode, int action, int mods) override;
};


#endif //BUILDIT_SAVEFEATURE_H
