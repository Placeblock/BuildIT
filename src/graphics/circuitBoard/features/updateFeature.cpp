//
// Created by felix on 9/14/24.
//

#include "updateFeature.h"

void UpdateFeature::update(float deltaTime) {
    std::set<Network*> updated;
    for (auto &component: *this->componentContainer->getComponents()) {
        if (Node *node = dynamic_cast<Node*>(component.get())) {
            if (node->resetUpdated()) {
                for (int i = 0; i < node->outputPins.size(); ++i) {
                    const intVec2 outputPinCell = intVec2(node->getPos() / 32.0f) + intVec2(node->outputPins[i]);
                    if (const auto joint = this->cabling->getJoint(outputPinCell); joint != nullptr) {
                        if (updated.contains(joint->getNetwork())) continue;
                        updated.insert(joint->getNetwork());
                        joint->getNetwork()->update();
                    }
                }
            }
        }
    }
}

UpdateFeature::UpdateFeature(Cabling *cabling, ComponentContainer *componentContainer)
    : cabling(cabling), componentContainer(componentContainer) {

}
