//
// Created by felix on 8/11/24.
//

#ifndef BUILDIT_NODELIST_H
#define BUILDIT_NODELIST_H

#include "graphics/font/fontRenderer.h"
#include "graphics/gui/widgets/verticalList.h"

struct NodeElementData {
    std::string name;
    std::string icon;
};

class NodeList : public GUI::VerticalList {
public:
    NodeList(GUI::View* view, uintVec2 size, const std::vector<NodeElementData>& elements, Element* parent = nullptr);
};


#endif //BUILDIT_NODELIST_H
