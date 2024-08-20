//
// Created by felix on 8/7/24.
//

#include "circuitBoard.h"

#include <memory>
#include "graphics/circuitBoard/history/actions/moveVertexAction.h"
#include "graphics/circuitBoard/history/actions/createVertexAction.h"
#include "graphics/circuitBoard/history/actions/createWireAction.h"
#include "graphics/circuitBoard/history/actions/insertVertexAction.h"

void CircuitBoard::render() {
    GUI::Image::render();
    this->cursor.update(this->mousePos, this->camera);
    if (this->dragging) {
        this->onDrag();
    }

    this->useFrameBuffer();
    this->programs->gridProgram->setVec2("resolution", this->getSize());
    this->programs->updateZoomUniforms(this->getSize(), this->camera);

    cursorRenderer.update(this->cursor.pos);

    gridRenderer.render(this->programs->gridProgram);
    nodes.pinRenderer.render(this->programs->pinProgram);

    this->programs->vertexProgram->setFloat("size", 15.0);
    wiresRenderer.render(this->programs->wireProgram, this->programs->vertexProgram);

    if (this->visualize) {
        this->visWiresRenderer.render(this->programs->wireProgram, this->programs->vertexProgram);
    }

    this->programs->vertexProgram->setFloat("size", 15.0);
    cursorRenderer.render(this->programs->vertexProgram);
}

CircuitBoard::CircuitBoard(Programs *programs, GUI::View *view, uintVec2 size)
    : programs(programs), selection(Selection{&this->wires, &this->wiresRenderer}), FrameBufferRenderable(size),
      GUI::Image(view, size, this->frameTexture, false){

}

void CircuitBoard::updateSize(uintVec2 newSize) {
    GUI::Image::updateSize(newSize);
    this->updateFrameBufferSize(newSize);
}

void CircuitBoard::onMouseMove(uintVec2 relPos, uintVec2 delta) {
	this->mousePos = relPos;
    if (this->navigating) {
		this->camera.target -= glm::vec2(delta)*this->camera.getZoomScalar();
    }
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

void CircuitBoard::onMouseAction(uintVec2 relPos, int button, int mouseAction) {
    if (button == GLFW_MOUSE_BUTTON_RIGHT) {
        this->navigating = mouseAction == GLFW_PRESS;
    } else if (button == GLFW_MOUSE_BUTTON_LEFT) {
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
    this->clickedVertex = this->wires.getVertex(this->cursor.hoveringCell);
    if ((this->shift && clickedVertex == nullptr) ||
        !this->shift && (clickedVertex != nullptr || this->wires.getWire(this->clickedCell) != nullptr)) {
        this->action = modWires;
        this->visVertices.push_back(std::make_unique<Vertex>(this->clickedCell, glm::vec3(0, 100, 100)));
    } else if (this->shift) {
        this->action = moveVertex;
        this->selection.addVertex(this->clickedVertex);
    }
    this->visualize = true;
}

void CircuitBoard::onClick() {
    if (this->clickedVertex != nullptr) {
        if (!this->shift) {
            this->selection.clear();
        }
        this->selection.addVertex(this->clickedVertex);
    } else {
        this->selection.clear();
    }
}

void CircuitBoard::onDragSubmit() {
    if (this->action == moveVertex) {
        const intVec2 delta = this->cursor.hoveringCell - this->clickedCell;
        for (const auto &item: this->selection.vertices) {
            const intVec2 newPos = intVec2(item->cell) + delta;
            const Vertex* newPosVertex = this->wires.getVertex(newPos);
            if (newPosVertex != nullptr && !this->selection.vertices.contains(newPosVertex)) return;
            // CHECK IF ANY VERTEX IS ON A WIRE
        }
        this->history.startBatch();
        for (const auto &item: this->selection.vertices) {
            std::unique_ptr<Action> dAction = std::make_unique<MoveVertexAction>(this->wires.getOwningRef(item), intVec2(item->cell) + delta, &this->wires, &this->wiresRenderer);
            this->history.dispatch(dAction);
        }
        this->history.endBatch();
    } else if (this->action == modWires) {
        const intVec2 endCell = this->calculateEndCell();
        this->history.startBatch();
        Vertex* start = this->clickedVertex;
        Vertex* end = this->wires.getVertex(endCell);
        if (start == nullptr) {
            start = this->visVertices[0].get();
            this->createOrInsertVertex(this->visVertices[0]);
        }
        if (end == nullptr) {
            end = this->visVertices[1].get();
            this->createOrInsertVertex(this->visVertices[1]);
        }
        this->visWires[0]->start = start;
        this->visWires[0]->end = end;
        std::unique_ptr<Action> dAction = std::make_unique<CreateWireAction>(std::move(this->visWires[0]), &this->wires, &this->wiresRenderer, false);
        this->history.dispatch(dAction);
        this->history.endBatch();
    }
}

void CircuitBoard::onDragStart() {
    if (this->action == modWires) {
        this->visVertices.push_back(std::make_unique<Vertex>(this->cursor.hoveringCell, glm::vec3(0, 100, 100)));
        this->visWires.push_back(std::make_unique<Wire>(this->visVertices[0].get(), this->visVertices[1].get(), glm::vec3(0, 100, 100)));
    } else if (this->action == moveVertex) {
        for (const auto &vertex: this->selection.vertices) {
            this->visVertices.push_back(std::make_unique<Vertex>(vertex->cell, glm::vec3(100, 100, 0)));
        }
        int i = 0;
        for (const auto &vertex: this->selection.vertices) {
            for (const auto &wire: vertex->wires) {
                Vertex* otherVertex = wire->getOther(vertex);
                if (this->selection.vertices.contains(otherVertex)) {
                    const auto iter = this->selection.vertices.find(otherVertex);
                    long index = std::distance(this->selection.vertices.begin(), iter);
                    this->visWires.push_back(std::make_unique<Wire>(this->visVertices[index].get(), this->visVertices[i].get(), glm::vec3(100, 100, 0)));
                } else {
                    this->visWires.push_back(std::make_unique<Wire>(otherVertex, this->visVertices[i].get(), glm::vec3(100, 100, 0)));
                }
            }
            i++;
        }
    }
}

void CircuitBoard::onDrag() {
    if (this->action == modWires) {
        const intVec2 endCell = this->calculateEndCell();
        this->visVertices[1]->cell = endCell;
    } else if (this->action == moveVertex) {
        const glm::vec2 delta = this->cursor.pos/32.0f - glm::vec2(this->clickedCell);
        int i = 0;
        for (const auto &vertex: this->selection.vertices) {
            this->visVertices[i]->cell = vertex->cell + delta;
            i++;
        }
    }
    this->updateVisWires();
}

void CircuitBoard::onDragEnd() {
    if (this->action == modWires) {
        this->visWires.clear();
        this->visVertices.resize(1);
    } else if (this->action == moveVertex) {
        this->visWires.clear();
        this->visVertices.clear();
    }
    this->updateVisWires();
}

void CircuitBoard::createOrInsertVertex(std::unique_ptr<Vertex>& vertex) {
    vertex->cell = glm::round(vertex->cell);
    std::unique_ptr<Action> dAction;
    if (this->wires.getWire(vertex->cell) != nullptr) {
        dAction = std::make_unique<InsertVertexAction>(std::move(vertex), &this->wires, &this->wiresRenderer, false);
    } else {
        dAction = std::make_unique<CreateVertexAction>(std::move(vertex), &this->wires, &this->wiresRenderer, false);
    }
    this->history.dispatch(dAction);
}


void CircuitBoard::onKeyAction(uintVec2 relPos, int key, int scanCode, int keyAction, int mods) {
	if (key == GLFW_KEY_LEFT_SHIFT) {
        this->shift = keyAction == GLFW_PRESS;
    } else if (key == GLFW_KEY_LEFT_CONTROL) {
        this->ctrl = keyAction == GLFW_PRESS;
    } else if (key == GLFW_KEY_ESCAPE) {
        this->resetAction();
        this->selection.clear();
    } else if (key == GLFW_KEY_DELETE) {
        if (!this->selection.vertices.empty()) {
            this->history.startBatch();
            for (const auto &vertex: this->selection.vertices) {
                auto wIter = vertex->wires.begin();
                while (wIter != vertex->wires.end()) {
                    std::unique_ptr<Action> dAction = std::make_unique<CreateWireAction>(this->wires.getOwningRef(*wIter++), &this->wires, &this->wiresRenderer, true);
                    this->history.dispatch(dAction);
                }
                std::unique_ptr<Action> dAction = std::make_unique<CreateVertexAction>(this->wires.getOwningRef(vertex), &this->wires, &this->wiresRenderer, true);
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
    this->clickedVertex = nullptr;
    this->visWires.clear();
    this->visVertices.clear();
    this->action = nothing;
    this->updateVisWires();
}


void CircuitBoard::onScroll(uintVec2 relPos, glm::vec2 offset) {
    glm::vec2 worldMousePos = this->camera.screenToWorld(this->mousePos);
    this->camera.target = worldMousePos;
    this->camera.offset = -this->mousePos;
    this->camera.zoom+= 0.1f*float(offset.y)*this->camera.zoom;
    this->programs->updateZoomUniforms(this->getSize(), this->camera);
}

void CircuitBoard::updateVisWires() {
    std::set<const Vertex*> vertices;
    std::transform(this->visVertices.begin(), this->visVertices.end(), std::inserter(vertices, vertices.end()), [](const auto& v) {
        return v.get();
    });
    std::set<const Wire*> wireData;
    std::transform(this->visWires.begin(), this->visWires.end(), std::inserter(wireData, wireData.end()), [](const auto& v) {
        return v.get();
    });
    this->visWiresRenderer.regenerateData(vertices, wireData);
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
    //std::cout << endPos.x << " | " << endPos.y << " || " << this->clickedCell.x << " | " << this->clickedCell.y << "\n";
    return endPos;
}
