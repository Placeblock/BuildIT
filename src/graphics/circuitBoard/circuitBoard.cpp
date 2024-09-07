//
// Created by felix on 8/7/24.
//

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "circuitBoard.h"

#include <memory>
#include "graphics/circuitBoard/history/actions/moveJointAction.h"
#include "graphics/circuitBoard/history/actions/createJointAction.h"
#include "graphics/circuitBoard/history/actions/createWireAction.h"
#include "graphics/circuitBoard/history/actions/insertJointAction.h"
#include "graphics/util.h"

void CircuitBoard::prerender(Programs* programs) {
    std::set<Network*> updated;
    for (const auto &node: this->nodes.joints) {
        if (node.second->resetUpdated()) {
            for (int i = 0; i < node.second->outputPins.size(); ++i) {
                const glm::vec2 outputPinCell = node.second->pos + glm::vec2(node.second->outputPins[i]);
                if (const auto joint = this->wires.getJoint(outputPinCell); joint != nullptr) {
                    if (updated.contains(joint->network)) continue;
                    updated.insert(joint->network);
                    joint->network->update();
                }
            }
        }
    }

    GUI::Image::prerender(programs);
    if (this->dragging) {
        this->onDrag();
    }

    this->useFrameBuffer();
    programs->gridProgram->setVec2("resolution", this->getSize());
    programs->updateZoomUniforms(this->getSize(), this->camera);

    gridRenderer.render(programs->gridProgram);

    fontRenderer.render(programs->textureProgram);


    programs->vertexProgram->setFloat("size", 15.0);

    if (this->visualize) {
        this->visWiresRenderer.render(programs->wireProgram, programs->vertexProgram);
    }

    programs->vertexProgram->setFloat("size", 15.0);
}

CircuitBoard::CircuitBoard(GUI::View *view, uintVec2 size, Sim::Simulation* simulation)
    : simulation(simulation), simBridge(SimulationFeature(this->simulation, &this->nodes, &this->wires, &this->wiresRenderer)),
      selection(Selection{&this->simBridge, &this->wires, &this->wiresRenderer}),
      fontRenderer(FontRenderer(view->font)),
      FrameBufferRenderable(size),
      GUI::Image(view, size, this->frameTexture, false) {

}

void CircuitBoard::updateSize(uintVec2 newSize) {
    GUI::Image::updateSize(newSize);
    this->updateFrameBufferSize(newSize);
}

void CircuitBoard::onMouseMove(glm::vec2 relPos, glm::vec2 delta) {
    if (this->action != nothing) {
        if (this->cursor.hoveringCell != this->clickedCell && !this->dragging) {
            this->dragging = true;
            this->onDragStart();
        } else if (this->cursor.hoveringCell == this->clickedCell && this->dragging) {
            this->dragging = false;
            this->onDragEnd();
        }
    }
}

void CircuitBoard::onMouseAction(glm::vec2 relPos, int button, int mouseAction, int mods) {
    if (button == GLFW_MOUSE_BUTTON_LEFT) {
        if (mouseAction == GLFW_PRESS) {
            this->onMouseDown();
        } else {
            if (this->cursor.hoveringCell == this->clickedCell) {
                this->onClick();
            } else if (this->dragging) {
                this->onDragSubmit();
                this->dragging = false;
            }
            this->resetAction();
        }
    }
}

void CircuitBoard::onMouseDown() {
    this->clickedCell = this->cursor.hoveringCell;
    this->clickedJoint = this->wires.getJoint(this->cursor.hoveringCell);
    bool modWiresNoShift = this->canModWiresNoShift(this->clickedCell);
    if (this->clickedJoint != nullptr && this->shift) {
        this->action = moveVertex;
        this->selection.addJoint(this->clickedJoint);
        this->visualize = true;
        return;
    }
    if ((this->shift || modWiresNoShift) && this->canModWires(this->clickedCell)) {
        this->action = modWires;
        this->visJoints.push_back(std::make_unique<Joint>(this->clickedCell, this->visNetwork.get()));
        this->visualize = true;
        return;
    }
}

bool CircuitBoard::canModWiresNoShift(intVec2 cell) {
    return this->wires.getJoint(cell) != nullptr ||
           this->nodes.inputPins.contains(cell) ||
           this->nodes.outputPins.contains(cell) ||
           this->wires.getWire(cell) != nullptr;
}

bool CircuitBoard::canModWires(intVec2 cell) {
    return !this->nodes.isOccupied(cell, {}) ||
            this->nodes.inputPins.contains(cell) ||
            this->nodes.outputPins.contains(cell);
}


void CircuitBoard::onClick() {
    if (this->clickedJoint != nullptr) {
        if (!this->shift) {
            this->selection.clear();
        }
        this->selection.addJoint(this->clickedJoint);
    } else {
        this->selection.clear();
    }
}

void CircuitBoard::onDragSubmit() {
    if (this->action == moveVertex) {
        const intVec2 delta = this->cursor.hoveringCell - this->clickedCell;
        for (const auto &item: this->selection.joints) {
            const intVec2 newPos = intVec2(item->pos) + delta;
            const Joint* newPosJoint = this->wires.getJoint(newPos);
            if (newPosJoint != nullptr && !this->selection.joints.contains(newPosJoint)) return;
            // CHECK IF ANY VERTEX IS ON A WIRE
        }
        this->history.startBatch();
        for (const auto &joint: this->selection.joints) {
            std::unique_ptr<Action> dAction = std::make_unique<MoveJointAction>(&this->simBridge, &this->wires,
                                                                                &this->wiresRenderer,
                                                                                this->wires.getOwningRef(joint),
                                                                                intVec2(joint->pos) + delta);
            this->history.dispatch(dAction);
        }
        this->history.endBatch();
    } else if (this->action == modWires) {
        const intVec2 endCell = this->calculateEndCell();
        this->history.startBatch();
        Joint* start = this->clickedJoint;
        Joint* end = this->wires.getJoint(endCell);
        if (start == nullptr) {
            start = this->visJoints[0].get();
            this->createOrInsertJoint(this->visJoints[0]);
        }
        if (end == nullptr) {
            end = this->visJoints[1].get();
            this->createOrInsertJoint(this->visJoints[1]);
        }
        this->visWires[0]->start = start;
        this->visWires[0]->end = end;
        std::unique_ptr<Action> dAction = std::make_unique<CreateWireAction>(this->simulation, &this->wires,
                                                                             &this->wires, &this->simBridge,
                                                                             &this->wiresRenderer, std::move(this->visWires[0]),
                                                                             false);
        this->history.dispatch(dAction);
        this->history.endBatch();
    }
}

void CircuitBoard::onDragStart() {
    if (this->action == modWires) {
        this->visJoints.push_back(std::make_unique<Joint>(this->cursor.hoveringCell, this->visNetwork.get()));
        this->visWires.push_back(std::make_unique<Wire>(this->visJoints[0].get(), this->visJoints[1].get(), this->visNetwork.get()));
    } else if (this->action == moveVertex) {
        for (const auto &joint: this->selection.joints) {
            this->visJoints.push_back(std::make_unique<Joint>(joint->pos, this->visNetwork.get()));
        }
        int i = 0;
        for (const auto &joint: this->selection.joints) {
            for (const auto &wire: joint->wires) {
                Joint* otherJoint = wire->getOther(joint);
                if (this->selection.joints.contains(otherJoint)) {
                    const auto iter = this->selection.joints.find(otherJoint);
                    long index = std::distance(this->selection.joints.begin(), iter);
                    this->visWires.push_back(std::make_unique<Wire>(this->visJoints[index].get(), this->visJoints[i].get(), this->visNetwork.get()));
                } else {
                    this->visWires.push_back(std::make_unique<Wire>(otherJoint, this->visJoints[i].get(), this->visNetwork.get()));
                }
            }
            i++;
        }
    }
}

void CircuitBoard::onDrag() {
    if (this->action == modWires) {
        const intVec2 endCell = this->calculateEndCell();
        this->visJoints[1]->pos = endCell;
    } else if (this->action == moveVertex) {
        const glm::vec2 delta = this->cursor.pos/32.0f - glm::vec2(this->clickedCell);
        int i = 0;
        for (const auto &vertex: this->selection.joints) {
            this->visJoints[i]->pos = vertex->pos + delta;
            i++;
        }
    }
    this->updateVisWires();
}

void CircuitBoard::onDragEnd() {
    if (this->action == modWires) {
        this->visWires.clear();
        this->visJoints.resize(1);
    } else if (this->action == moveVertex) {
        this->visWires.clear();
        this->visJoints.clear();
    }
    this->updateVisWires();
}

void CircuitBoard::createOrInsertJoint(std::unique_ptr<Joint> &joint) {
    joint->pos = glm::round(joint->pos);
    std::unique_ptr<Action> dAction;
    if (this->wires.getWire(joint->pos) != nullptr) {
        dAction = std::make_unique<InsertJointAction>(&this->wires, &this->simBridge,
                                                      &this->wiresRenderer, std::move(joint), false);
    } else {
        dAction = std::make_unique<CreateJointAction>(&this->simBridge, &this->wires,
                                                      &this->wiresRenderer, std::move(joint), false);
    }
    this->history.dispatch(dAction);
}


void CircuitBoard::onKeyAction(glm::vec2 relPos, int key, int scanCode, int keyAction, int mods) {
	if (key == GLFW_KEY_LEFT_SHIFT) {
        this->shift = keyAction == GLFW_PRESS;
    } else if (key == GLFW_KEY_LEFT_CONTROL) {
        this->ctrl = keyAction == GLFW_PRESS;
    } else if (key == GLFW_KEY_ESCAPE) {
        this->resetAction();
        this->selection.clear();
    } else if (key == GLFW_KEY_DELETE) {
        if (!this->selection.joints.empty()) {
            this->history.startBatch();
            for (const auto &vertex: this->selection.joints) {
                auto wIter = vertex->wires.begin();
                while (wIter != vertex->wires.end()) {
                    std::unique_ptr<Action> dAction = std::make_unique<CreateWireAction>(this->simulation, &this->wires,
                                                                                         &this->wires, &this->simBridge,
                                                                                         &this->wiresRenderer, this->wires.getOwningRef(*wIter++),
                                                                                         true);
                    this->history.dispatch(dAction);
                }
                std::unique_ptr<Action> dAction = std::make_unique<CreateJointAction>(&this->simBridge, &this->wires, &this->wiresRenderer,
                                                                                      this->wires.getOwningRef(vertex), true);
                this->history.dispatch(dAction);
            }
            this->history.endBatch();
        }
        this->resetAction();
        this->selection.clear();
    } else if (key == GLFW_KEY_Y) {
        if (this->ctrl && keyAction == GLFW_PRESS) {
            this->history.undo();
            this->resetAction();
            this->selection.clear();
        }
    } else if (key == GLFW_KEY_Z) {
        if (this->ctrl && keyAction == GLFW_PRESS) {
            this->history.redo();
            this->resetAction();
            this->selection.clear();
        }
    }
}

void CircuitBoard::resetAction() {
    this->visualize = false;
    this->dragging = false;
    this->clickedJoint = nullptr;
    this->visWires.clear();
    this->visJoints.clear();
    this->action = nothing;
    this->updateVisWires();
}

void CircuitBoard::updateVisWires() {
    std::set<const Joint*> joints;
    std::transform(this->visJoints.begin(), this->visJoints.end(), std::inserter(joints, joints.end()), [](const auto& v) {
        return v.get();
    });
    std::set<const Wire*> wireData;
    std::transform(this->visWires.begin(), this->visWires.end(), std::inserter(wireData, wireData.end()), [](const auto& v) {
        return v.get();
    });
    this->visWiresRenderer.regenerateData(joints, wireData);
}

intVec2 CircuitBoard::calculateEndCell() {
    const float startDistance = glm::distance(glm::vec2(this->clickedCell), glm::vec2(this->cursor.hoveringCell));
    intVec2 endPos;
    float endPosDistance = -1;

    for (int x = -1; x <= 1; x++) {
        for (int y = -1; y <= 1; y++) {
            if (x == 0 && y == 0) continue;
            const glm::vec2 delta = glm::normalize(glm::vec2(x, y));
            const intVec2 cEndPos = this->clickedCell + intVec2(std::round(delta.x * startDistance), std::round(delta.y * startDistance));
            const float cEndPosDistance = glm::distance(glm::vec2(cEndPos), this->cursor.pos/32.0f);
            if (endPosDistance == -1 || endPosDistance > cEndPosDistance) {
                endPosDistance = cEndPosDistance;
                endPos = cEndPos;
            }
        }
    }
    return endPos;
}

void CircuitBoard::addNode(std::unique_ptr<Node> node) {
    this->nodes.addNode(std::move(node));
}

void CircuitBoard::update(Node *node) {
    for (const auto &item: node->outputPins) {

    }
}
