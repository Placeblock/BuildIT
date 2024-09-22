//
// Created by felix on 8/7/24.
//

#include "circuitBoard.h"

#include "graphics/circuitBoard/features/historyFeature.h"
#include "graphics/circuitBoard/features/navigationFeature.h"
#include "graphics/circuitBoard/features/deleteFeature.h"
#include "graphics/circuitBoard/features/cursorFeature.h"
#include "graphics/circuitBoard/features/cablingFeature.h"
#include "graphics/circuitBoard/features/modifyCablingFeature.h"
#include "graphics/circuitBoard/features/moveFeature.h"
#include "graphics/circuitBoard/features/nodesFeature.h"
#include "graphics/circuitBoard/features/simulationFeature.h"
#include "graphics/circuitBoard/features/updateFeature.h"


void CircuitBoard::prerender(Programs* programs) {
    const float delta = float(glfwGetTime() - this->lastTime) * 1000.0f;
    this->lastTime = glfwGetTime();

    for (const auto &updatable: this->updatableFeatures) {
        updatable->update(delta);
    }

    Image::prerender(programs);

    this->useFrameBuffer();
    programs->gridProgram->setVec2("resolution", this->getSize());
    programs->updateZoomUniforms(this->getSize(), this->camera);
    programs->updateProjectionUniforms(this->getSize(), this->camera);

    gridRenderer.render(programs->gridProgram);

    this->componentRenderers.render(programs);
    for (const auto &renderable: this->renderableFeatures) {
        renderable->render();
    }

    fontRenderer.render(programs->textureProgram);
}

CircuitBoard::CircuitBoard(Programs *programs, GUI::View *view, const uintVec2 size, Sim::Simulation* simulation)
    : FrameBufferRenderable(size), Image(view, size, this->frameTexture, false), componentRenderers(&this->fontRenderer),
      simulation(simulation), fontRenderer(FontRenderer(&view->font)) {

    auto *historyFeature = new HistoryFeature(&this->history);
    this->features.push_back(historyFeature);

    auto *navigationFeature = new NavigationFeature(&this->camera);
    this->features.push_back(navigationFeature);

    this->cursorFeature = new CursorFeature(programs, &this->camera, this);
    this->features.push_back(this->cursorFeature);
    this->updatableFeatures.push_back(this->cursorFeature);
    this->renderableFeatures.push_back(this->cursorFeature);

    auto *selectionFeature = new SelectionFeature(programs, this->cursorFeature, &this->collisionDetection);
    this->history.subscribe(selectionFeature);
    this->features.push_back(selectionFeature);
    this->renderableFeatures.push_back(selectionFeature);

    auto *deleteFeature = new DeleteFeature(selectionFeature, &this->history, &this->components);
    this->features.push_back(deleteFeature);

    const auto cablingFeature = new CablingFeature(&this->history, &this->components, &this->componentRenderers.cablingRenderer);
    this->features.push_back(cablingFeature);

    const auto modifyCablingFeature = new ModifyCablingFeature(programs, &this->history, &this->collisionDetection, selectionFeature,
                                                               this->cursorFeature, &this->components, &cablingFeature->cabling,
                                                               &cablingFeature->networks);
    this->features.push_back(modifyCablingFeature);
    this->cursorFeature->subscribe(modifyCablingFeature);
    this->history.subscribe(modifyCablingFeature);
    this->renderableFeatures.push_back(modifyCablingFeature);

    const auto moveFeature = new MoveFeature(programs, &this->history, &this->camera, &this->FrameBufferRenderable::size,
                                             &this->collisionDetection, selectionFeature, this->cursorFeature, &this->fontRenderer);
    this->features.push_back(moveFeature);
    this->history.subscribe(moveFeature);
    this->renderableFeatures.push_back(moveFeature);

    const auto nodesFeature = new NodesFeature(programs, &this->components, &this->components);
    this->features.push_back(nodesFeature);
    this->renderableFeatures.push_back(nodesFeature);

    const auto simulationFeature = new SimulationFeature(this->simulation, nodesFeature->getNodePinHandler(), &cablingFeature->cabling);
    this->features.push_back(simulationFeature);
    this->components.Subject<ComponentAddEvent>::subscribe(simulationFeature);
    this->components.Subject<ComponentRemoveEvent>::subscribe(simulationFeature);
    cablingFeature->Subject<NetworksSplitEvent>::subscribe(simulationFeature);
    cablingFeature->Subject<NetworksMergeEvent>::subscribe(simulationFeature);

    const auto updateFeature = new UpdateFeature(&cablingFeature->cabling, &this->components);
    this->features.push_back(updateFeature);
    this->updatableFeatures.push_back(updateFeature);

    this->components.Subject<ComponentAddEvent>::subscribe(this);
    this->components.Subject<ComponentRemoveEvent>::subscribe(this);
}

void CircuitBoard::updateSize(uintVec2 newSize) {
    Image::updateSize(newSize);
    this->updateFrameBufferSize(newSize);
}

glm::vec2 CircuitBoard::getMousePos() {
    return this->view->mousePos - glm::vec2(this->getAbsPos());
}

void CircuitBoard::onMouseMove(const glm::vec2 relPos, const glm::vec2 delta) {
    for (const auto &item: this->features) {
        item->onMouseMove(relPos, delta);
    }
}

void CircuitBoard::onMouseAction(const glm::vec2 relPos, const int button, const int action, const int mods) {
    for (const auto &item: this->features) {
        item->onMouseAction(relPos, button, action, mods);
    }
}

void CircuitBoard::onScroll(const glm::vec2 relPos, const glm::vec2 offset) {
    for (const auto &item: this->features) {
        item->onScroll(relPos, offset);
    }
}

void CircuitBoard::onKeyAction(const glm::vec2 relPos, const int key, const int scanCode, const int action, const int mods) {
    for (const auto &item: this->features) {
        item->onKeyAction(relPos, key, scanCode, action, mods);
    }
}

void CircuitBoard::onChar(const glm::vec2 relPos, const unsigned char c) {
    for (const auto &item: this->features) {
        item->onChar(relPos, c);
    }
}

void CircuitBoard::notify(const ComponentAddEvent &data) {
    if (const auto interactable = dynamic_cast<Interactable*>(data.component)) {
        this->collisionDetection.addElement(interactable);
    }
    RendererAddVisitor addVisitor{&this->componentRenderers};
    data.component->visit(&addVisitor);
}

void CircuitBoard::notify(const ComponentRemoveEvent &data) {
    if (const auto interactable = dynamic_cast<Interactable*>(data.component)) {
        this->collisionDetection.removeElement(interactable);
    }
    RendererRemoveVisitor removeVisitor{&this->componentRenderers};
    data.component->visit(&removeVisitor);
}
