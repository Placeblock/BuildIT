//
// Created by felix on 8/31/24.
//

#ifndef BUILDIT_NODESFEATURE_H
#define BUILDIT_NODESFEATURE_H

#include "feature.h"
#include "graphics/circuitBoard/components/nodes/nodePins.h"
#include "graphics/circuitBoard/components/componentContainer.h"

/**
 * Adds functionality for rendering nodes on the CircuitBoard
 */
class NodesFeature : public Feature, public Renderable {
private:
    NodePins nodePins;
public:
    explicit NodesFeature(Programs *programs);

    NodePinHandler *getNodePinHandler();
    void render() override;
};


#endif //BUILDIT_NODESFEATURE_H
