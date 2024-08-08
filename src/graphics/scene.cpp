//
// Created by felix on 8/7/24.
//

#include "scene.h"
#include "graphics/history/actions/moveVertexAction.h"
#include "graphics/history/actions/createVertexAction.h"
#include "graphics/history/actions/createWireAction.h"
#include "graphics/history/actions/insertVertexAction.h"

void Scene::render() {
    glViewport(0,0,this->size.x, this->size.y);
    glBindFramebuffer(GL_FRAMEBUFFER, this->framebuffer);
    this->programs->updateProjectionUniforms(this->size, this->camera);

    cursorRenderer.update(this->cursor.pos);

    gridRenderer.render(this->programs->gridProgram);
    nodes.pinRenderer.render(this->programs->pinProgram);

    this->programs->vertexProgram->setFloat("size", 15.0);
    wiresRenderer.render(this->programs->wireProgram, this->programs->vertexProgram);

    this->programs->vertexProgram->setFloat("size", 15.0);
    cursorRenderer.render(this->programs->vertexProgram);
}

Scene::Scene(Programs *programs, vpSize size) : programs(programs), size(size) {
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

void Scene::onResize(vpSize newSize) {
    this->size = newSize;
    this->programs->gridProgram->setVec2("resolution", this->size);
    this->programs->updateProjectionUniforms(this->size, this->camera);
    glBindTexture(GL_TEXTURE_2D, this->texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, this->size.x, this->size.y, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
}

void Scene::onMouseMove(glm::vec2 abs, glm::vec2 delta) {
	this->mousePos = abs;
    this->cursor.update(abs, this->camera);
    if (this->dragging) {
		this->camera.target -= delta*this->camera.getZoomScalar();
    }
    if (this->action == createVertex) {
        if (this->cursor.hoveringCell != this->actionStart) {
            this->action = createVertexAndLine;
        } else {
            this->action = createVertex;
        }
    } else if (this->action == insertVertex) {
        if (this->cursor.hoveringCell != this->actionStart) {
            this->action = insertVertexAndLine;
        } else {
            this->action = insertVertex;
        }
    }
}

void Scene::onMouseAction(int button, int mouseAction, int mods) {
    if (button == GLFW_MOUSE_BUTTON_RIGHT) {
        this->dragging = mouseAction == GLFW_PRESS;
    } else if (button == GLFW_MOUSE_BUTTON_LEFT) {
        if (mouseAction == GLFW_PRESS) {
            this->actionStart = this->cursor.hoveringCell;
            if (this->wires.getVertex(this->actionStart) != nullptr) {
                this->action = moveVertex;
            } else if (this->wires.getWire(this->actionStart) != nullptr) {
                this->action = insertVertex;
            } else {
                this->action = createVertex;
            }
        } else {
            if (this->action == moveVertex) {
                if (this->cursor.hoveringCell == this->actionStart ||
                    this->wires.getVertex(this->cursor.hoveringCell) != nullptr) return;
                std::shared_ptr<Vertex> vertex = this->wires.getVertex(this->actionStart);
                std::shared_ptr<Wire> targetWire = this->wires.getWire(this->cursor.hoveringCell);
                if (targetWire != nullptr && !vertex->wires.contains(targetWire)) return;
                MoveVertexAction{vertex, this->cursor.hoveringCell}.Execute(&this->wires, &this->wiresRenderer, false);
            } else if (this->action == createVertex) {
                std::shared_ptr<Vertex> vertex = std::make_shared<Vertex>(this->actionStart, glm::vec3(100, 100, 0));
                CreateVertexAction{vertex}.Execute(&this->wires, &this->wiresRenderer, true);
            } else if (action == insertVertex) {
                std::shared_ptr<Vertex> vertex = std::make_shared<Vertex>(this->actionStart, glm::vec3(100, 100, 0));
                InsertVertexAction{vertex}.Execute(&this->wires, &this->wiresRenderer, true);
            } else if (action == insertVertexAndLine || createVertexAndLine) {
                std::shared_ptr<Vertex> start = std::make_shared<Vertex>(this->actionStart, glm::vec3(100, 100, 0));
                if (action == createVertexAndLine) {
                    CreateVertexAction{start}.Execute(&this->wires, &this->wiresRenderer, true);
                } else {
                    InsertVertexAction{start}.Execute(&this->wires, &this->wiresRenderer, true);
                }
                std::shared_ptr<Vertex> end = this->wires.getVertex(this->cursor.hoveringCell);
                if (end == nullptr) {
                    std::shared_ptr<Wire> endWire = this->wires.getWire(this->cursor.hoveringCell);
                    if (endWire == nullptr) {
                        end = std::make_shared<Vertex>(this->cursor.hoveringCell, glm::vec3(100, 100, 0), start->network);
                        CreateVertexAction{end}.Execute(&this->wires, &this->wiresRenderer, true);
                    } else {
                        end = std::make_shared<Vertex>(this->cursor.hoveringCell, glm::vec3(100, 100, 0), endWire->network);
                        InsertVertexAction{end}.Execute(&this->wires, &this->wiresRenderer, true);
                    }
                } else {
                    start->network = end->network;
                }
                std::shared_ptr<Wire> wire = std::make_shared<Wire>(start, end, glm::vec3(0, 100, 100));
                CreateWireAction{wire}.Execute(&this->wires, &this->wiresRenderer, true);
            }
        }
    }
}

void Scene::onKeyAction(int key, int scanCode, int action, int mods) {
	
}

void Scene::onScroll(glm::vec2 offset) {
    glm::vec2 worldMousePos = this->camera.screenToWorld(this->mousePos);
    this->camera.target = worldMousePos;
    this->camera.offset = -this->mousePos;
    this->camera.zoom+= 0.1f*float(offset.y)*this->camera.zoom;
    this->programs->updateZoomUniforms(this->size, this->camera);
}
