//
// Created by felix on 8/19/24.
//

#ifndef BUILDIT_NODEELEMENT_H
#define BUILDIT_NODEELEMENT_H


#include "graphics/gui/widgets/verticalList.h"

class NodeElement : public GUI::VerticalList {
public:
    NodeElement(GUI::View* view, const std::string& textureFile, const std::string& name);
};


#endif //BUILDIT_NODEELEMENT_H
