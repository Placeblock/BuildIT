//
// Created by felix on 8/21/24.
//

#ifndef BUILDIT_NOTNODEELEMENT_H
#define BUILDIT_NOTNODEELEMENT_H


#include "nodeElement.h"
#include "graphics/circuitBoard/renderer/node/instancedNodeRenderer.h"
#include "graphics/circuitBoard/elements/nodes/gate.h"

class NotNodeElement : public NodeElement {
private:
    InstancedNodeRenderer<Gate>* renderer;
public:
    NotNodeElement(GUI::View* view, NodeReceiver* nodeReceiver, InstancedNodeRenderer<Gate>* renderer, Sim::Simulation* simulation);
    std::unique_ptr<Node> createNode(glm::vec2 absPos) override;

    void postrender(Programs* programs) override;
};


#endif //BUILDIT_NOTNODEELEMENT_H
