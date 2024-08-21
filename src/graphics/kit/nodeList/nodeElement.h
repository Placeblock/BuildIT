//
// Created by felix on 8/19/24.
//

#ifndef BUILDIT_NODEELEMENT_H
#define BUILDIT_NODEELEMENT_H


#include "graphics/gui/widgets/verticalList.h"
#include "nodeReceiver.h"

class NodeElement : public GUI::VerticalList {
private:
    NodeReceiver* nodeReceiver;
public:
    NodeElement(GUI::View* view, const std::string& name, NodeReceiver* nodeReceiver);

    void onMouseAction(glm::vec2 relPos, int button, int mouseAction) override;

    virtual std::unique_ptr<Node> createNode() = 0;
};


#endif //BUILDIT_NODEELEMENT_H
