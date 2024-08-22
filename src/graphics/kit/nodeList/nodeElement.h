//
// Created by felix on 8/19/24.
//

#ifndef BUILDIT_NODEELEMENT_H
#define BUILDIT_NODEELEMENT_H


#include "graphics/gui/widgets/verticalList.h"
#include "nodeDragHandler.h"
#include "simulation/simulation.h"
#include "graphics/circuitBoard/renderer/node/nodeRenderer.h"
#include "graphics/circuitBoard/circuitBoard.h"
#include "graphics/gui/widgets/text.h"

template <class N, class R>
class NodeElement : public GUI::VerticalList, public NodeAdder {
static_assert(std::is_base_of<Node, N>::value, "N must derive from Node");
static_assert(std::is_base_of<NodeRenderer<N>, R>::value, "R must derive from NodeRenderer<N>");

private:
    NodeDragHandler* nodeDragHandler;
    std::unique_ptr<N> movingNode;
protected:
    Sim::Simulation* simulation;
    R* renderer;
public:
    NodeElement(GUI::View* view, const std::string& name, NodeDragHandler* nodeDragHandler,
                Sim::Simulation* simulation, R *renderer);

    void onMouseAction(glm::vec2 relPos, int button, int mouseAction) override;

    std::unique_ptr<Node> addNode(CircuitBoard *board) override;
    void moveNode(glm::vec2 absPos) override;
    void removeNode() override;

    virtual std::unique_ptr<N> createNode(glm::vec2 absPos) = 0;
    virtual R* getTargetRenderer(CircuitBoard *board) = 0;

    void postrender(Programs *programs) override;

    ~NodeElement() override {
        delete this->renderer;
    }
};

template <class N, class R>
NodeElement<N, R>::NodeElement(GUI::View *view, const std::string& name, NodeDragHandler* nodeDragHandler,
                               Sim::Simulation* simulation, R *renderer)
        : GUI::VerticalList(view, uintVec2(160, 176)), nodeDragHandler(nodeDragHandler),
          simulation(simulation), renderer(renderer) {
    std::unique_ptr<GUI::Element> title = std::make_unique<GUI::Text>(view, uintVec2(160, 0),
                                                                      name, Alignment::CENTER, Color{255, 255, 0},
                                                                      16);
    this->addChild(title);
}

template <class N, class R>
void NodeElement<N, R>::onMouseAction(glm::vec2 relPos, int button, int mouseAction) {
    Container::onMouseAction(relPos, button, mouseAction);
    if (button != GLFW_MOUSE_BUTTON_LEFT || mouseAction != GLFW_PRESS) return;
    this->movingNode = this->createNode(glm::vec2(this->getAbsPos()) + relPos);
    this->movingNode->addToRenderer();
    this->nodeDragHandler->setActiveNodeAdder(this);
}

template<class N, class R>
void NodeElement<N, R>::moveNode(glm::vec2 absPos) {
    this->movingNode->onMove(absPos, true);
}

template<class N, class R>
void NodeElement<N, R>::removeNode() {
    this->movingNode = nullptr;
}

template<class N, class R>
std::unique_ptr<Node> NodeElement<N, R>::addNode(CircuitBoard *board) {
    glm::vec2 worldPos = board->cursor.hoveringCell * 32;
    this->movingNode->onMove(worldPos, false);
    this->movingNode->removeFromRenderer();
    this->getTargetRenderer(board)->addNode(this->movingNode.get());
    // SET TARGET RENDERER IN NODE
    return std::move(this->movingNode);
}

template<class N, class R>
void NodeElement<N, R>::postrender(Programs *programs) {
    if (this->movingNode != nullptr) {
        Camera tcamera{this->movingNode->pos, -this->movingNode->pos, this->nodeDragHandler->getBoardZoom()};
        programs->updateProjectionUniforms(this->view->root->getSize(), tcamera);
        this->renderer->render(programs);
    }
    Element::postrender(programs);
}


#endif //BUILDIT_NODEELEMENT_H
