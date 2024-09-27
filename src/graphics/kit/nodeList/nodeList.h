//
// Created by felix on 8/11/24.
//

#ifndef BUILDIT_NODELIST_H
#define BUILDIT_NODELIST_H

#include "graphics/gui/widgets/verticalList.h"
#include "nodeDragHandler.h"
#include "simulation/simulation.h"

class NodeList final : public GUI::VerticalList {
public:
    NodeList(GUI::View* view, uintVec2 size, NodeDragHandler* nodeReceiver);
};


#endif //BUILDIT_NODELIST_H
