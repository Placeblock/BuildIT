//
// Created by felix on 8/1/24.
//

#include "graphics.h"
#include <iostream>
#include "shader.h"
#include "renderer/wiresRenderer.h"
#include "renderer/gridRenderer.h"
#include "renderer/cursorRenderer.h"
#include "cursor.h"
#include "history/actions/createWireAction.h"
#include "history/actions/createVertexAction.h"
#include "history/actions/insertVertexAction.h"
#include "history/actions/moveVertexAction.h"

#include <glm/gtc/matrix_transform.hpp>

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    auto* graphics = static_cast<Graphics*>(glfwGetWindowUserPointer(window));
    glViewport(0, 0, width, height);
    graphics->updateShaderUniforms();
}

void scroll_callback(GLFWwindow* window, double _, double yOffset) {
    auto* graphics = static_cast<Graphics*>(glfwGetWindowUserPointer(window));
    glm::vec2 mousePos = graphics->getMousePos();
    glm::vec2 worldMousePos = graphics->camera.screenToWorld(mousePos);
    graphics->camera.target = worldMousePos;
    graphics->camera.offset = -mousePos;
    graphics->camera.zoom+= 0.1f*float(yOffset);
    graphics->updateShaderUniforms();
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
    auto* graphics = static_cast<Graphics*>(glfwGetWindowUserPointer(window));
    if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS) {
        graphics->interaction->handleRightClick();
    } else if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        graphics->interaction->handleLeftClick();
    }
}

void Graphics::init() {
    glfwInit();

    this->window = createWindow();
    if (this->window == nullptr) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return;
    }
    glfwMakeContextCurrent(this->window);
    glfwSetWindowUserPointer(this->window, this);
    glViewport(0, 0, 640, 480);
    glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);

    GLenum err = glewInit();
    if (GLEW_OK != err) {
        /* Problem: glewInit failed, something is seriously wrong. */
        printf("Error: %s\n", glewGetErrorString(err));
    }

    this->vertexProgram = new Shader("resources/shaders/circleVertexShader.vs",
                                     "resources/shaders/pointFragmentShader.fs",
                                     "resources/shaders/pointGeometryShader.gs");
    this->wireProgram = new Shader("resources/shaders/defaultVertexShader.vs",
                                   "resources/shaders/defaultFragmentShader.fs",
                                   "resources/shaders/wireGeometryShader.gs");
    this->gridProgram = new Shader("resources/shaders/defaultVertexShader.vs",
                                   "resources/shaders/gridShader.fs");
    this->updateShaderUniforms();

    glfwSetFramebufferSizeCallback(this->window, framebuffer_size_callback);
    glfwSetScrollCallback(this->window, scroll_callback);

    Wires wires;

    WiresRenderer wiresRenderer;
    wiresRenderer.init();

    CreateVertexAction{std::make_shared<Vertex>(glm::vec2(5, 5), glm::vec3(0.5, 0.2, 0.5))}.Execute(&wires, &wiresRenderer, false);
    CreateVertexAction{std::make_shared<Vertex>(glm::vec2(11, 5), glm::vec3(0.5, 0.2, 0.5))}.Execute(&wires, &wiresRenderer, false);
    CreateWireAction{std::make_shared<Wire>((*wires.vertexMap.begin()).first, (*(++wires.vertexMap.begin())).first, glm::vec3(0.5, 0.2, 0.5))}.Execute(&wires, &wiresRenderer, false);
    InsertVertexAction{std::make_shared<Vertex>(glm::vec2(8, 5), glm::vec3(0.5, 0.2, 0.5))}.Execute(&wires, &wiresRenderer, true);

    GridRenderer gridRenderer;
    gridRenderer.init();
    CursorRenderer cursorRenderer;
    cursorRenderer.init();

    Cursor cursor;

    glEnable(GL_PROGRAM_POINT_SIZE);

    bool dragging;
    glm::vec2 oldDragPos = glm::vec2(-1, -1);
    bool moving;
    std::shared_ptr<Vertex> movingVertex;
    std::shared_ptr<Vertex> lastHoveredVertex;

    while(!glfwWindowShouldClose(this->window)) {
        bool shiftClick = glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS;
        this->interaction->update(this->getMousePos(), cursor.cursorPos, shiftClick);

        std::shared_ptr<Vertex> hoveredVertex = wires.getVertex(cursor.hoveringCell);
        if (hoveredVertex != nullptr) {
            hoveredVertex->color = glm::vec3(0.7, 0.6, 0.2);
            lastHoveredVertex = hoveredVertex;
            wiresRenderer.updateVertexColor(wires.getVertexIndex(hoveredVertex), hoveredVertex->color);
        }
        if (lastHoveredVertex != nullptr && hoveredVertex == nullptr || lastHoveredVertex != hoveredVertex) {
            lastHoveredVertex->color = lastHoveredVertex->network->color;
            wiresRenderer.updateVertexColor(wires.getVertexIndex(lastHoveredVertex), lastHoveredVertex->color);
            lastHoveredVertex = nullptr;
        }

        int rightMouseState = glfwGetMouseButton(this->window, GLFW_MOUSE_BUTTON_RIGHT);
        dragging = rightMouseState == GLFW_PRESS;
        if (dragging) {
            glm::vec2 newMousePos = this->getMousePos();
            if (oldDragPos.x != -1 && oldDragPos.y != -1) {
                glm::vec2 delta = newMousePos - oldDragPos;
                this->camera.target -= delta*this->camera.getZoomScalar();
                this->updateShaderUniforms();
            }
            oldDragPos = newMousePos;
        } else {
            oldDragPos = glm::vec2(-1, -1);
        }
        int leftMouseState = glfwGetMouseButton(this->window, GLFW_MOUSE_BUTTON_LEFT);
        if (leftMouseState == GLFW_PRESS && !moving) {
            movingVertex = hoveredVertex;
        }
        moving = leftMouseState == GLFW_PRESS;
        if (moving && movingVertex != nullptr
            && movingVertex->cell != cursor.hoveringCell && hoveredVertex == nullptr) {
            std::shared_ptr<Wire> hoveredWire = wires.getWire(cursor.hoveringCell);
            if (hoveredWire == nullptr || movingVertex->wires.contains(hoveredWire)) {
                MoveVertexAction{movingVertex, cursor.hoveringCell}.Execute(&wires, &wiresRenderer, false);
            }
        }

        cursor.update(this->getMousePos(), this->camera);
        cursorRenderer.update(cursor.cursorPos);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        gridRenderer.render(this->gridProgram);
        this->vertexProgram->setFloat("cSize", 25.0, true);
        wiresRenderer.render(this->wireProgram, this->vertexProgram);
        //this->interaction->renderVis(this->wireProgram, this->vertexProgram);
        this->vertexProgram->setFloat("cSize", 15.0, false);
        if (!hoveredVertex) {
            cursorRenderer.render(this->vertexProgram);
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glfwTerminate();
}

GLFWwindow *Graphics::createWindow() {
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    return glfwCreateWindow(640, 480, "BuildIT", nullptr, nullptr);
}

void Graphics::updateShaderUniforms() {
    int windowWidth, windowHeight;
    glfwGetWindowSize(this->window, &windowWidth, &windowHeight);
    glm::mat4 projectionMat = this->camera.getProjectionMat(glm::vec2(windowWidth, windowHeight));
    this->wireProgram->setMat4("projection", projectionMat, true);
    this->vertexProgram->setMat4("projection", projectionMat, true);
    this->vertexProgram->setFloat("zoom", this->camera.zoom, false);
    this->gridProgram->setVec2("offset", this->camera.getPos(), true);
    this->gridProgram->setVec2("resolution", glm::vec2(windowWidth, windowHeight), false);
    this->gridProgram->setFloat("zoom", this->camera.zoom, false);
}

glm::vec2 Graphics::getMousePos() const {
    double x, y;
    glfwGetCursorPos(this->window, &x, &y);
    return {x, y};
}