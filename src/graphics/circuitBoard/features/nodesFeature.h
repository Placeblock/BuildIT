//
// Created by felix on 8/31/24.
//

#ifndef BUILDIT_NODESFEATURE_H
#define BUILDIT_NODESFEATURE_H

#include "feature.h"
#include "graphics/circuitBoard/components/nodes/nodes.h"
#include "graphics/circuitBoard/components/nodes/renderer/nodeRenderers.h"
#include "graphics/circuitBoard/components/nodes/nodePins.h"

/**
 * Adds functionality for rendering joints on the CircuitBoard
 */
class NodesFeature : public Feature, public Renderable {
private:
    NodeContainer nodes;
    NodeInteractionManager nodeInteractionManager;
    NodeRenderers nodeRenderers;
    NodePins nodePins;
public:
    NodesFeature(Programs *programs, FontRenderer *fontRenderer);
    void render() override;
};


#endif //BUILDIT_NODESFEATURE_H
