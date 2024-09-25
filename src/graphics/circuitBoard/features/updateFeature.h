//
// Created by felix on 9/14/24.
//

#ifndef BUILDIT_UPDATEFEATURE_H
#define BUILDIT_UPDATEFEATURE_H


#include "feature.h"
#include "graphics/circuitBoard/components/cabling/cabling.h"
#include "graphics/circuitBoard/components/componentContainer.h"
#include "graphics/threadPool.h"

class UpdateFeature final : public Feature, public Updatable {
    std::unordered_set<Node*>* nodes;

    ThreadPool updatePool;
public:
    UpdateFeature(std::unordered_set<Node*>* nodes);

    void update(float deltaTime) override;
    static void updateNode(Node *node);
};


#endif //BUILDIT_UPDATEFEATURE_H
