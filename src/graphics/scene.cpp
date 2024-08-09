//
// Created by felix on 8/7/24.
//

#include "scene.h"
#include "graphics/history/actions/moveVertexAction.h"
#include "graphics/history/actions/createVertexAction.h"
#include "graphics/history/actions/createWireAction.h"
#include "graphics/history/actions/insertVertexAction.h"

void Scene::render() {
    this->cursor.update(this->mousePos, this->camera);
    if (this->dragging) {
        this->onDrag();
    }

    glViewport(0,0,this->size.x, this->size.y);
    glBindFramebuffer(GL_FRAMEBUFFER, this->framebuffer);
    this->programs->updateProjectionUniforms(this->size, this->camera);

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

Scene::Scene(Programs *programs, intVec2 size) : programs(programs), size(size), selection(Selection{&this->wires, &this->wiresRenderer}) {
    glGenFramebuffers(1, &this->framebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, this->framebuffer);
    glGenTextures(1, &this->texture);
    glBindTexture(GL_TEXTURE_2D, this->texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, this->size.x, this->size.y, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glBindTexture(GL_TEXTURE_2D, 0);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, this->texture, 0);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Scene::use() {
    this->programs->gridProgram->setVec2("resolution", this->size);
    this->programs->updateZoomUniforms(this->size, this->camera);
}

void Scene::onResize(intVec2 newSize) {
    this->size = newSize;
    this->programs->gridProgram->setVec2("resolution", this->size);
    this->programs->updateProjectionUniforms(this->size, this->camera);
    glBindTexture(GL_TEXTURE_2D, this->texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, this->size.x, this->size.y, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
}

void Scene::onMouseMove(glm::vec2 abs, glm::vec2 delta) {
	this->mousePos = abs;
    if (this->navigating) {
		this->camera.target -= delta*this->camera.getZoomScalar();
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

void Scene::onMouseAction(int button, int mouseAction, int mods) {
    if (button == GLFW_MOUSE_BUTTON_RIGHT) {
        this->navigating = mouseAction == GLFW_PRESS;
    } else if (button == GLFW_MOUSE_BUTTON_LEFT) {
        if (mouseAction == GLFW_PRESS) {
            this->onMouseDown();
        } else {
            if (this->cursor.hoveringCell == this->clickedCell) {
                this->onClick();
            } else {
                this->onDragSubmit();
            };
            this->resetAction();
        }
    }
}

void Scene::onMouseDown() {
    this->clickedCell = this->cursor.hoveringCell;
    this->clickedVertex = this->wires.getVertex(this->cursor.hoveringCell);
    if ((this->shift && clickedVertex == nullptr) ||
        !this->shift && (clickedVertex != nullptr || this->wires.getWire(this->clickedCell) != nullptr)) {
        this->action = modWires;
        this->visVertices.push_back(std::make_shared<Vertex>(this->clickedCell, glm::vec3(0, 100, 100)));
    } else if (this->shift) {
        if (this->clickedVertex == nullptr) return;
        this->action = moveVertex;
        this->selection.addVertex(this->clickedVertex);
    }
    this->visualize = true;
}

void Scene::onClick() {
    if (this->clickedVertex != nullptr) {
        if (!this->shift) {
            this->selection.clear();
        }
        this->selection.addVertex(this->clickedVertex);
    } else {
        this->selection.clear();
    }
}

void Scene::onDragSubmit() {
    if (this->action == moveVertex) {
        const intVec2 delta = this->cursor.hoveringCell - this->clickedCell;
        for (const auto &item: this->selection.vertices) {
            const intVec2 newPos = intVec2(item->cell) + delta;
            const std::shared_ptr<Vertex> newPosVertex = this->wires.getVertex(newPos);
            if (newPosVertex != nullptr && !this->selection.vertices.contains(newPosVertex)) return;
            // CHECK IF ANY VERTEX IS ON A WIRE
        }
        for (const auto &item: this->selection.vertices) {
            MoveVertexAction{item, intVec2(item->cell) + delta}.Execute(&this->wires, &this->wiresRenderer, false);
        }
    } else if (this->action == modWires) {
        const intVec2 endCell = this->calculateEndCell();
        if (this->clickedVertex == nullptr) {
            this->createOrInsertVertex(this->visVertices[0]);
        }
        if (this->wires.getVertex(endCell) == nullptr) {
            this->createOrInsertVertex(this->visVertices[1]);
        }
        this->visWires[0]->start = this->wires.getVertex(this->clickedCell);
        this->visWires[0]->end = this->wires.getVertex(endCell);
        CreateWireAction{this->visWires[0]}.Execute(&this->wires, &this->wiresRenderer, true);
    }
}

void Scene::onDragStart() {
    if (this->action == modWires) {
        this->visVertices.push_back(std::make_shared<Vertex>(this->cursor.hoveringCell, glm::vec3(0, 100, 100)));
        this->visWires.push_back(std::make_shared<Wire>(this->visVertices[0], this->visVertices[1], glm::vec3(0, 100, 100)));
    } else if (this->action == moveVertex) {
        for (const auto &vertex: this->selection.vertices) {
            this->visVertices.push_back(std::make_shared<Vertex>(vertex->cell, glm::vec3(100, 100, 0)));
        }
        int i = 0;
        for (const auto &vertex: this->selection.vertices) {
            for (const auto &wire: vertex->wires) {
                const std::shared_ptr<Vertex> otherVertex = wire->getOther(vertex);
                if (this->selection.vertices.contains(otherVertex)) {
                    const auto iter = this->selection.vertices.find(otherVertex);
                    long index = std::distance(this->selection.vertices.begin(), iter);
                    this->visWires.push_back(std::make_shared<Wire>(this->visVertices[index], this->visVertices[i], glm::vec3(100, 100, 0)));
                } else {
                    this->visWires.push_back(std::make_shared<Wire>(otherVertex, this->visVertices[i], glm::vec3(100, 100, 0)));
                }
            }
            i++;
        }
    }
}

void Scene::onDrag() {
    if (this->action == modWires) {
        const intVec2 endCell = this->calculateEndCell();
        this->visVertices[1]->cell = endCell;
    } else if (this->action == moveVertex) {
        const glm::vec2 delta = this->cursor.pos/32.0f - glm::vec2(this->clickedCell);
        int i = 0;
        for (const auto &vertex: this->selection.vertices) {
            const std::shared_ptr<Vertex> visVertex = this->visVertices[i];
            visVertex->cell = vertex->cell + delta;
            i++;
        }
    }
    this->updateVisWires();
}

void Scene::onDragEnd() {
    if (this->action == modWires) {
        this->visWires.clear();
        this->visWires.resize(1);
    } else if (this->action == moveVertex) {
        this->visWires.clear();
        this->visVertices.clear();
    }
    this->updateVisWires();
}

void Scene::createOrInsertVertex(const std::shared_ptr<Vertex>& vertex) {
    vertex->cell = glm::round(vertex->cell);
    if (this->wires.getWire(vertex->cell) != nullptr) {
        InsertVertexAction{vertex}.Execute(&this->wires, &this->wiresRenderer, true);
    } else {
        CreateVertexAction{vertex}.Execute(&this->wires, &this->wiresRenderer, true);
    }
}


void Scene::onKeyAction(int key, int scanCode, int keyAction, int mods) {
	if (key == GLFW_KEY_LEFT_SHIFT) {
        this->shift = keyAction == GLFW_PRESS;
    } else if (key == GLFW_KEY_ESCAPE) {
        this->selection.clear();
        this->resetAction();
    }
}

void Scene::resetAction() {
    this->visualize = false;
    this->dragging = false;
    this->visWires.clear();
    this->visVertices.clear();
    this->action = nothing;
    this->updateVisWires();
}


void Scene::onScroll(glm::vec2 offset) {
    glm::vec2 worldMousePos = this->camera.screenToWorld(this->mousePos);
    this->camera.target = worldMousePos;
    this->camera.offset = -this->mousePos;
    this->camera.zoom+= 0.1f*float(offset.y)*this->camera.zoom;
    this->programs->updateZoomUniforms(this->size, this->camera);
}

void Scene::updateVisWires() {
    std::set<std::shared_ptr<Vertex>> vertices(this->visVertices.begin(), this->visVertices.end());
    std::set<std::shared_ptr<Wire>> wireData(this->visWires.begin(), this->visWires.end());
    this->visWiresRenderer.regenerateData(&vertices, &wireData);
}

intVec2 Scene::calculateEndCell() {
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
