//
// Created by felix on 8/31/24.
//

#ifndef BUILDIT_NODESFEATURE_H
#define BUILDIT_NODESFEATURE_H

#include "feature.h"
#include "graphics/circuitBoard/components/nodes/nodes.h"
#include "graphics/circuitBoard/components/nodes/renderer/nodeRenderers.h"

/**
 * Adds functionality for rendering nodes on the CircuitBoard
 */
class NodesFeature : public Feature, public Renderable {
private:
    NodeInteractionManager nodes{};
    NodeRenderers nodeRenderers;
public:
    NodesFeature(Programs *programs, FontRenderer *fontRenderer);
    void render() override;
};


#endif //BUILDIT_NODESFEATURE_H
