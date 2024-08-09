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
    if (this->action == moveVertex) {
        this->visVertices[0]->cell = this->cursor.pos / 32.0f;
        this->updateVisWires();
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

Scene::Scene(Programs *programs, intVec2 size) : programs(programs), size(size) {
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
    if (this->dragging) {
		this->camera.target -= delta*this->camera.getZoomScalar();
    }
    if (this->action == modWires) {
        const intVec2 endCell = this->calculateEndCell();
        if (endCell != this->actionCell && this->cursor.hoveringCell != this->actionCell) {
            if (this->visVertices.size() == 1) {
                this->visVertices.push_back(std::make_shared<Vertex>(endCell, glm::vec3(0, 100, 0)));
            } else {
                this->visVertices[1]->cell = endCell;
            }
            if (this->visWires.size() == 0) {
                this->visWires.push_back(std::make_shared<Wire>(this->visVertices[0], this->visVertices[1], glm::vec3(100, 0, 0)));
            }
        } else {
            this->visVertices.resize(1);
            this->visWires.clear();
        }
        this->updateVisWires();
    }
}

void Scene::onMouseAction(int button, int mouseAction, int mods) {
    if (button == GLFW_MOUSE_BUTTON_RIGHT) {
        this->dragging = mouseAction == GLFW_PRESS;
    } else if (button == GLFW_MOUSE_BUTTON_LEFT) {
        if (mouseAction == GLFW_PRESS) {
            this->actionCell = this->cursor.hoveringCell;
            const std::shared_ptr<Vertex> vertex = this->wires.getVertex(this->cursor.hoveringCell);
            if (!this->shift) {
                this->action = modWires;
                this->visualize = true;
                this->visVertices.push_back(std::make_shared<Vertex>(this->actionCell, glm::vec3(0, 100, 0)));
                this->updateVisWires();
            } else if (vertex != nullptr) {
                this->action = moveVertex;
                this->visualize = true;
                const std::shared_ptr<Vertex> visVertex = std::make_shared<Vertex>(this->actionCell, glm::vec3(0, 100, 0));
                this->visVertices.push_back(visVertex);
                for (const auto &item: vertex->wires) {
                    this->visWires.push_back(std::make_shared<Wire>(item->getOther(vertex), visVertex, glm::vec3(100, 100, 0)));
                }
                this->updateVisWires();
            }
        } else {
            if (this->action == moveVertex) {
                const std::shared_ptr<Vertex> vertex = this->wires.getVertex(this->actionCell);
                if (this->cursor.hoveringCell != intVec2(vertex->cell) &&
                    this->wires.getVertex(this->cursor.hoveringCell) == nullptr) {
                    const std::shared_ptr<Wire> targetWire = this->wires.getWire(this->cursor.hoveringCell);
                    if (targetWire == nullptr || vertex->wires.contains(targetWire)) {
                        MoveVertexAction{vertex, this->cursor.hoveringCell}.Execute(&this->wires, &this->wiresRenderer, false);
                    }
                }
            } else if (this->action == modWires) {
                if (this->wires.getVertex(this->actionCell) == nullptr) {
                    this->createOrInsertVertex(this->visVertices[0]);
                }
                const intVec2 endCell = this->calculateEndCell();
                if (this->cursor.hoveringCell != this->actionCell && endCell != this->actionCell) {
                    if (this->wires.getVertex(endCell) == nullptr) {
                        this->createOrInsertVertex(this->visVertices[1]);
                    }
                    this->visWires[0]->start = this->wires.getVertex(this->actionCell);
                    this->visWires[0]->end = this->wires.getVertex(endCell);
                    CreateWireAction{this->visWires[0]}.Execute(&this->wires, &this->wiresRenderer, true);
                }
            }
            this->visualize = false;
            this->visWires.clear();
            this->visVertices.clear();
            this->action = nothing;
        }
    }
}

void Scene::createOrInsertVertex(const std::shared_ptr<Vertex>& vertex) {
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
        this->visualize = false;
        this->visWires.clear();
        this->visVertices.clear();
        this->action = nothing;
    }
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
    const float startDistance = glm::distance(glm::vec2(this->actionCell), glm::vec2(this->cursor.hoveringCell));
    intVec2 endPos;
    float endPosDistance = -1;

    for (int x = -1; x <= 1; x++) {
        for (int y = -1; y <= 1; y++) {
            if (x == 0 && y == 0) continue;
            const glm::vec2 delta = glm::normalize(glm::vec2(x, y));
            const intVec2 cEndPos = this->actionCell + intVec2(std::round(delta.x * startDistance), std::round(delta.y * startDistance));
            const float cEndPosDistance = glm::distance(glm::vec2(cEndPos), this->cursor.pos/32.0f);
            if (endPosDistance == -1 || endPosDistance > cEndPosDistance) {
                endPosDistance = cEndPosDistance;
                endPos = cEndPos;
            }
        }
    }
    //std::cout << endPos.x << " | " << endPos.y << " || " << this->actionCell.x << " | " << this->actionCell.y << "\n";
    return endPos;
}