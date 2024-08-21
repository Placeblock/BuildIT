//
// Created by felix on 8/11/24.
//

#ifndef BUILDIT_NODELIST_H
#define BUILDIT_NODELIST_H

#include "graphics/font/fontRenderer.h"
#include "graphics/gui/widgets/verticalList.h"
#include "nodeReceiver.h"

struct NodeElementData {
    std::string name;
    std::string icon;
};

class NodeList : public GUI::VerticalList, public NodeReceiver {
public:
    NodeList(GUI::View* view, uintVec2 size, const std::vector<NodeElementData>& elements);
    void receiveNode(glm::vec2 pos, std::unique_ptr<Node> node) override;
};


#endif //BUILDIT_NODELIST_H
