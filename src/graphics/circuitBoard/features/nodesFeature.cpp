//
// Created by felix on 8/31/24.
//

#include "nodesFeature.h"

NodesFeature::NodesFeature(Programs *programs, FontRenderer *fontRenderer)
    : Renderable(programs), nodeRenderers({NotNodeRenderer{fontRenderer}}),
      nodeInteractionManager(&nodes, &nodes){

}

void NodesFeature::render() {
    nodeRenderers.notNode.render(programs);
    nodes.pinRenderer.render(programs->pinProgram);
}
