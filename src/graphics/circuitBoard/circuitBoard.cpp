//
// Created by felix on 8/7/24.
//

#include <fstream>
#include <sstream>
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
#include "graphics/circuitBoard/features/copyFeature.h"
#include "graphics/circuitBoard/history/actions/createComponentAction.h"
#include "simulation/updaters.h"
#include "graphics/circuitBoard/serialization/deserialize/binaryDeserializer.h"
#include "graphics/circuitBoard/features/saveFeature.h"


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
                                                               this->cursorFeature, &this->components, &cablingFeature->cabling);
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

    const auto updateFeature = new UpdateFeature(nodesFeature->getNodes());
    this->features.push_back(updateFeature);
    this->updatableFeatures.push_back(updateFeature);

    const auto copyFeature = new CopyFeature(&this->history, selectionFeature, cursorFeature,
                                             &this->components);
    this->features.push_back(copyFeature);

    const auto saveFeature = new SaveFeature(&this->components, this->simulation);
    this->features.push_back(saveFeature);

    this->components.Subject<ComponentAddEvent>::subscribe(this);
    this->components.Subject<ComponentRemoveEvent>::subscribe(this);

    /*for (int x = 0; x < 80; ++x) {
        for (int y = 0; y < 80; ++y) {
            this->createNotLoop({x*32*6, y*32*4});
        }
    }*/

    std::fstream fs;
    fs.open("save.buildit", std::fstream::in);
    if (!fs.fail()) {
        std::stringstream is;
        is << fs.rdbuf();
        BinaryDeserializer deserializer;
        deserializer.deserialize(is);
        for (const auto &component: deserializer.components) {
            this->components.addComponent(component);
        }
        this->simulation->clearUpdateQueue();
        while (!deserializer.updateQueue.empty()) {
            this->simulation->update(deserializer.updateQueue.front());
            deserializer.updateQueue.pop();
        }
    } else {
        std::cout << "COULD NOT LOAD FILE!\n";
    }
}

void CircuitBoard::createNotLoop(glm::vec2 pos) {
    std::shared_ptr<Network> network = std::make_shared<Network>();
    std::shared_ptr<Joint> topLeftJoint = std::make_shared<Joint>(pos, network);
    std::shared_ptr<Joint> topRightJoint = std::make_shared<Joint>(pos + glm::vec2(32*5, 0), network);
    std::shared_ptr<Joint> bottomLeftJoint = std::make_shared<Joint>(pos + glm::vec2(0, 32*2), network);
    std::shared_ptr<Joint> bottomRightJoint = std::make_shared<Joint>(pos + glm::vec2(32*5, 32*2), network);
    std::shared_ptr<Joint> nodeLeftJoint = std::make_shared<Joint>(pos + glm::vec2(32*1, 0), network);
    std::shared_ptr<Joint> nodeRightJoint = std::make_shared<Joint>(pos + glm::vec2(32*4, 0), network);
    network->joints.push_back(topLeftJoint.get());
    network->joints.push_back(topRightJoint.get());
    network->joints.push_back(bottomLeftJoint.get());
    network->joints.push_back(bottomRightJoint.get());
    network->joints.push_back(nodeLeftJoint.get());
    network->joints.push_back(nodeRightJoint.get());
    std::shared_ptr<Wire> bottomWire = std::make_shared<Wire>(bottomLeftJoint.get(), bottomRightJoint.get(), network);
    bottomWire->connect();
    network->wires.push_back(bottomWire.get());
    std::shared_ptr<Wire> leftWire = std::make_shared<Wire>(topLeftJoint.get(), bottomLeftJoint.get(), network);
    leftWire->connect();
    network->wires.push_back(leftWire.get());
    std::shared_ptr<Wire> rightWire = std::make_shared<Wire>(topRightJoint.get(), bottomRightJoint.get(), network);
    rightWire->connect();
    network->wires.push_back(rightWire.get());
    std::shared_ptr<Wire> topLeftWire = std::make_shared<Wire>(topLeftJoint.get(), nodeLeftJoint.get(), network);
    topLeftWire->connect();
    network->wires.push_back(topLeftWire.get());
    std::shared_ptr<Wire> topRightWire = std::make_shared<Wire>(topRightJoint.get(), nodeRightJoint.get(), network);
    topRightWire->connect();
    network->wires.push_back(topRightWire.get());
    this->components.addComponent(nodeLeftJoint);
    this->components.addComponent(topLeftJoint);
    this->components.addComponent(bottomLeftJoint);
    this->components.addComponent(bottomRightJoint);
    this->components.addComponent(topRightJoint);
    this->components.addComponent(nodeRightJoint);
    this->components.addComponent(bottomWire);
    this->components.addComponent(leftWire);
    this->components.addComponent(rightWire);
    this->components.addComponent(topLeftWire);
    this->components.addComponent(topRightWire);
    std::shared_ptr<NotGate> notGate = std::make_shared<NotGate>(pos + glm::vec2(1*32, -1*32), std::make_shared<Sim::Node>(1, 1, std::make_unique<Sim::NotUpdater>()));
    this->components.addComponent(notGate);
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
