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

template <class N>
class NodeElement : public GUI::VerticalList, public NodeAdder {
static_assert(std::is_base_of<Node, N>::value, "N must derive from Node");

private:
    NodeDragHandler* nodeDragHandler;
protected:
    Sim::Simulation* simulation;
    std::unique_ptr<N> movingNode;
public:
    NodeElement(GUI::View* view, const std::string& name, NodeDragHandler* nodeDragHandler,
                Sim::Simulation* simulation);

    void onMouseAction(glm::vec2 relPos, int button, int mouseAction, int mods) override;

    std::unique_ptr<Node> addNode(CircuitBoard *board) override;
    void moveNode(glm::vec2 absPos) override;
    void removeNode() override;

    virtual std::unique_ptr<N> createNode(glm::vec2 absPos) = 0;

    void postrender(Programs *programs) override;

    ~NodeElement() override {
        delete this->renderer;
    }
};

template <class N>
NodeElement<N>::NodeElement(GUI::View *view, const std::string& name, NodeDragHandler* nodeDragHandler,
                               Sim::Simulation* simulation)
        : GUI::VerticalList(view, uintVec2(160, 176)), nodeDragHandler(nodeDragHandler),
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
    this->movingNode = this->createNode((glm::vec2(this->getAbsPos()) + relPos) / 32.0f);
    this->renderer->addNode(this->movingNode.get());
    this->nodeDragHandler->setActiveNodeAdder(this);
}

template<class N>
void NodeElement<N>::moveNode(glm::vec2 absPos) {
    this->movingNode->move(absPos, true);
}

template<class N>
void NodeElement<N>::removeNode() {
	this->renderer->removeNode(this->movingNode.get());
}

template<class N>
std::unique_ptr<Node> NodeElement<N>::addNode(CircuitBoard *board) {
    this->movingNode->move(board->cursorFeature->getHoveringCell());
    this->renderer->removeNode(this->movingNode.get());
    this->movingNode->renderer = this->getTargetRenderer(board);
    return std::move(this->movingNode);
}

template<class N>
void NodeElement<N>::postrender(Programs *programs) {
    if (this->movingNode != nullptr) {
        Camera tcamera{this->movingNode->getPos() * 32.0f, -this->movingNode->getPos() * 32.0f, this->nodeDragHandler->getBoardZoom()};
        programs->updateProjectionUniforms(this->view->root->getSize(), tcamera);
        this->renderer->render(programs);
    }
    Element::postrender(programs);
}


#endif //BUILDIT_NODEELEMENT_H
