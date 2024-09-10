//
// Created by felix on 8/19/24.
//

#ifndef BUILDIT_NODEELEMENT_H
#define BUILDIT_NODEELEMENT_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "graphics/gui/widgets/verticalList.h"
#include "graphics/circuitBoard/circuitBoard.h"
#include "graphics/gui/widgets/text.h"
#include "nodeDragHandler.h"
#include "graphics/circuitBoard/features/cursorFeature.h"

template <class C>
class NodeElement : public GUI::VerticalList {
static_assert(std::is_base_of<Component, C>::value, "N must derive from Node");

private:
    ComponentDragHandler* componentDragHandler;
protected:
    Sim::Simulation* simulation;
public:
    NodeElement(GUI::View* view, const std::string& name, ComponentDragHandler* nodeDragHandler,
                Sim::Simulation* simulation);

    void onMouseAction(glm::vec2 relPos, int button, int mouseAction, int mods) override;

    virtual std::unique_ptr<C> createNode(glm::vec2 absPos) = 0;
};

template <class N>
NodeElement<N>::NodeElement(GUI::View *view, const std::string& name, ComponentDragHandler* nodeDragHandler,
                            Sim::Simulation* simulation)
        : GUI::VerticalList(view, uintVec2(160, 176)), componentDragHandler(nodeDragHandler),
          simulation(simulation) {
    std::unique_ptr<GUI::Element> title = std::make_unique<GUI::Text>(view, uintVec2(160, 0),
                                                                      name, Alignment::CENTER, Color{255, 255, 0},
                                                                      16);
    this->addChild(title);
}

template <class N>
void NodeElement<N>::onMouseAction(glm::vec2 relPos, int button, int mouseAction, int mods) {
    Container::onMouseAction(relPos, button, mouseAction, mods);
    if (button != GLFW_MOUSE_BUTTON_LEFT || mouseAction != GLFW_PRESS) return;
    this->componentDragHandler->setCreatingComponent(this->createNode((glm::vec2(this->getAbsPos()) + relPos) / 32.0f));
}


#endif //BUILDIT_NODEELEMENT_H
