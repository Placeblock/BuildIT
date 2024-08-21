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

/*
 * Jedes NodeElement eigene Klasse. Die Klasse erzeugt dann die Node bei drag und
 * gibt sie per callback iwie ans Kit. Die Klasse bekommt nötige daten wie z.b. den
 * MeshRenderer für die jeweilige Node
 */

class Kit : public FrameBufferRenderable, public GUI::HorizontalList, public NodeReceiver {
public:
    Kit(GUI::View* view, Sim::Simulation* simulation, uintVec2 size);
private:
    glm::vec2 mousePos;
    Sim::Simulation* simulation;
    const Camera camera{}; // Default camera

    CircuitBoard* circuitBoard;
    NodeList* nodeList;

    uintVec2 calculateCBSize();
    uintVec2 calculateNLSize();

    std::unique_ptr<Node> createdNode;

    void updateSize(uintVec2 newSize) override;

    void receiveNode(glm::vec2 pos, std::unique_ptr<Node> node) override;
    void onMouseAction(glm::vec2 relPos, int button, int mouseAction) override;
    void onMouseMove(glm::vec2 relPos, glm::vec2 delta) override;
    void prerender(Programs* programs) override;
};


#endif //BUILDIT_KIT_H
