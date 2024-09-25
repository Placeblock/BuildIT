//
// Created by felix on 9/14/24.
//

#include "updateFeature.h"

#include <cmath>
#include "graphics/circuitBoard/components/nodes/node.h"
#include "graphics/util.h"

void UpdateFeature::update(float deltaTime) {
    uint nodesPerThread = uint(std::ceil(float(this->nodes->size()) / float(this->updatePool.getThreadCount())));
    auto start = this->nodes->begin();
    auto end = std::next(this->nodes->begin(), nodesPerThread);
    uint updatedNodes = 0;
    for (size_t i = 0; i < this->updatePool.getThreadCount(); ++i) {
        this->updatePool.queue([start, end](){
            auto startCopy = start;
            while (startCopy != end) {
                UpdateFeature::updateNode( *startCopy);
                startCopy++;
            }
        });
        updatedNodes += nodesPerThread;
        if (updatedNodes > this->nodes->size()) break;
        start = end;
        end = std::next(end, std::min(nodesPerThread, uint(this->nodes->size() - updatedNodes)));
    }
    this->updatePool.waitDone();
}

void UpdateFeature::updateNode(Node *node) {
    if (node->resetUpdated()) {
        for (size_t i = 0; i < node->outputPins.size(); ++i) {
            Network* outputNetwork = node->outputNetworks[i];
            if (outputNetwork == nullptr) continue;
            if (node->getOutput(i)) {
                outputNetwork->renderedColor = Util::hsv2rgb(outputNetwork->hsvColor - glm::vec3(0, 0.8, 0));
            } else {
                outputNetwork->renderedColor = Util::hsv2rgb(outputNetwork->hsvColor);
            }
            outputNetwork->update();
        }
    }
}

UpdateFeature::UpdateFeature(std::unordered_set<Node*>* nodes) : nodes(nodes), updatePool(20) {
    this->updatePool.start();
}
