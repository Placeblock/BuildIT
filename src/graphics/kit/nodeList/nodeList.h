//
// Created by felix on 8/11/24.
//

#ifndef BUILDIT_NODELIST_H
#define BUILDIT_NODELIST_H

#include "graphics/font/fontRenderer.h"
#include "graphics/gui/widgets/verticalList.h"
#include "nodeReceiver.h"
#include "simulation/simulation.h"

class NodeList : public GUI::VerticalList {
public:
    NodeList(GUI::View* view, uintVec2 size, Sim::Simulation* simulation, NodeReceiver* nodeReceiver);
};


#endif //BUILDIT_NODELIST_H
