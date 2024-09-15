//
// Created by felix on 8/10/24.
//

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "kit.h"
#include "image/stb_image.h"
#include "graphics/gui/widgets/text.h"
#include "graphics/circuitBoard/history/actions/createComponentAction.h"
#include "graphics/circuitBoard/features/cursorFeature.h"

void Kit::updateSize(uintVec2 newSize) {
    GUI::HorizontalList::updateSize(newSize);
    this->updateFrameBufferSize(newSize);
    this->nodeList->updateSize(this->calculateNLSize());
    this->circuitBoard->updateSize(this->calculateCBSize());
}

uintVec2 Kit::calculateCBSize() {
    return this->getSize() - uintVec2(160, 0);
}

uintVec2 Kit::calculateNLSize() {
    return {160, this->getSize().y};
}

Kit::Kit(Programs *programs, GUI::View* view, Sim::Simulation* simulation, uintVec2 size)
    : simulation(simulation), FrameBufferRenderable(size), creatingRenderers(&view->fontRenderer),
    GUI::HorizontalList(view, size) {

    std::unique_ptr<NodeList> lNodeList = std::make_unique<NodeList>(view, this->calculateNLSize(), this->simulation, this);
    this->nodeList = lNodeList.get();
    std::unique_ptr<CircuitBoard> lCircuitBoard = std::make_unique<CircuitBoard>(programs, view, this->calculateCBSize(), this->simulation);
    this->circuitBoard = lCircuitBoard.get();
    std::unique_ptr<GUI::Element> element1 = std::move(lNodeList);
    this->addChild(element1);
    std::unique_ptr<GUI::Element> element2 = std::move(lCircuitBoard);
    this->addChild(element2);
}

void Kit::setCreatingComponent(std::unique_ptr<Component> component) {
    this->creatingComponent = std::move(component);
    RendererAddVisitor addVisitor{&this->creatingRenderers};
    this->creatingComponent->visit(&addVisitor);
}

void Kit::onMouseAction(glm::vec2 relPos, int button, int mouseAction, int mods) {
    Container::onMouseAction(relPos, button, mouseAction, mods);
    if (button == GLFW_MOUSE_BUTTON_LEFT && mouseAction == GLFW_RELEASE
            && this->creatingComponent != nullptr) {
        // We remove the node from the node adder first because it gets invalidated next
        RendererRemoveVisitor removeVisitor{&this->creatingRenderers};
        this->creatingComponent->visit(&removeVisitor);
        this->creatingComponent->move(this->circuitBoard->cursorFeature->getHoveringCell() * 32);
        if (this->circuitBoard->mouseOver) {
            std::unique_ptr<Action> createAction = std::make_unique<CreateComponentAction>(&this->circuitBoard->components,
                                                                                           std::move(this->creatingComponent), false);
            History::dispatch(&this->circuitBoard->history, createAction);
        }
    }
}

void Kit::prerender(Programs *programs) {
    GUI::HorizontalList::prerender(programs);
    if (this->creatingComponent != nullptr) {
        if (this->circuitBoard->mouseOver) {
            const glm::vec2 cursorPos = this->circuitBoard->camera.worldToScreen(this->circuitBoard->cursorFeature->getCursorPos());
            const glm::vec2 nodePos = glm::vec2(this->circuitBoard->getAbsPos()) + cursorPos;
            this->creatingComponent->move(nodePos);
        } else {
            this->creatingComponent->move(this->view->mousePos);
        }
    }
}

void Kit::postrender(Programs *programs) {
    if (this->creatingComponent != nullptr) {
        Camera tcamera{this->creatingComponent->getPos(), -this->creatingComponent->getPos(), this->getBoardZoom()};
        programs->updateProjectionUniforms(this->view->root->getSize(), tcamera);
        this->creatingRenderers.render(programs);
    }
}

float Kit::getBoardZoom() {
    return this->circuitBoard->camera.zoom;
}
