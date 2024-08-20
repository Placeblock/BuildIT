//
// Created by felix on 8/10/24.
//

#ifndef BUILDIT_KIT_H
#define BUILDIT_KIT_H

#include "graphics/circuitBoard/circuitBoard.h"
#include "graphics/font/fontRenderer.h"
#include "graphics/kit/nodeList/nodeList.h"
#include "graphics/gui/widgets/horizontalList.h"
#include "simulation/simulation.h"


static const std::vector<NodeElementData> nodeElements = {
        NodeElementData{"Nicht", "gate_not.png"},
        NodeElementData{"Und", "gate_and.png"},
        NodeElementData{"Oder", "gate_or.png"},
};

class Kit : public FrameBufferRenderable, public GUI::HorizontalList {
public:
    Kit(GUI::View* view, Sim::Simulation* simulation, Programs* programs, uintVec2 size);
private:
    glm::vec2 mousePos;
    Sim::Simulation* simulation;
    const Camera camera{}; // Default camera

    CircuitBoard* circuitBoard;
    NodeList* nodeList;

    uintVec2 calculateCBSize();
    uintVec2 calculateNLSize();

    void updateSize(uintVec2 newSize) override;
};


#endif //BUILDIT_KIT_H
