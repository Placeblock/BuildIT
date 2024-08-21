//
// Created by felix on 8/19/24.
//

#ifndef BUILDIT_NODEELEMENT_H
#define BUILDIT_NODEELEMENT_H


#include "graphics/gui/widgets/verticalList.h"
#include "nodeReceiver.h"
#include "simulation/simulation.h"

class NodeElement : public GUI::VerticalList {
private:
    NodeReceiver* nodeReceiver;
protected:
    Sim::Simulation* simulation;
public:
    NodeElement(GUI::View* view, const std::string& name, NodeReceiver* nodeReceiver, Sim::Simulation* simulation);

    void onMouseAction(glm::vec2 relPos, int button, int mouseAction) override;

    virtual std::unique_ptr<Node> createNode(glm::vec2 absPos) = 0;
};


#endif //BUILDIT_NODEELEMENT_H
