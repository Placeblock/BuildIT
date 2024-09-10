//
// Created by felix on 8/21/24.
//

#ifndef BUILDIT_NOTNODEELEMENT_H
#define BUILDIT_NOTNODEELEMENT_H


#include "nodeElement.h"
#include "graphics/circuitBoard/components/nodes/gate.h"

class NotNodeElement : public NodeElement<Gate, NotGateRenderer> {
public:
    NotNodeElement(GUI::View* view, NodeDragHandler* nodeDragHandler, Sim::Simulation* simulation);

    std::unique_ptr<Gate> createNode(glm::vec2 absPos) override;
};


#endif //BUILDIT_NOTNODEELEMENT_H
