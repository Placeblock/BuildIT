//
// Created by felix on 8/10/24.
//

#ifndef BUILDIT_KIT_H
#define BUILDIT_KIT_H

#include "graphics/circuitBoard/circuitBoard.h"
#include "graphics/kit/nodeList/nodeList.h"
#include "graphics/gui/widgets/horizontalList.h"

class Kit : public FrameBufferRenderable, public GUI::HorizontalList, public NodeDragHandler {
public:
    Kit(GUI::View* view, Sim::Simulation* simulation, uintVec2 size);
private:
    Sim::Simulation* simulation;
    const Camera camera{}; // Default camera

    CircuitBoard* circuitBoard;
    NodeList* nodeList;

    uintVec2 calculateCBSize();
    uintVec2 calculateNLSize();

    NodeAdder* activeNodeAdder = nullptr;

    void updateSize(uintVec2 newSize) override;

    void setActiveNodeAdder(NodeAdder *adder) override;
    float getBoardZoom() override;

    void onMouseAction(glm::vec2 relPos, int button, int mouseAction) override;
    void prerender(Programs* programs) override;
};


#endif //BUILDIT_KIT_H
