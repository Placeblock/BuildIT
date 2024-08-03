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
#include "wires/actions/createWireAction.h"
#include "wires/actions/createVertexAction.h"

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

    this->lineJointsProgram = new Shader("resources/shaders/wireJointVertexShader.vs",
                                   "resources/shaders/pointFragmentShader.fs",
                                   "resources/shaders/pointGeometryShader.gs");
    this->lineProgram = new Shader("resources/shaders/defaultVertexShader.vs",
                                   "resources/shaders/defaultFragmentShader.fs",
                                   "resources/shaders/wireGeometryShader.gs");
    this->gridProgram = new Shader("resources/shaders/defaultVertexShader.vs",
                                   "resources/shaders/gridShader.fs");
    this->cursorProgram = new Shader("resources/shaders/cursorVertexShader.vs",
                                   "resources/shaders/pointFragmentShader.fs",
                                     "resources/shaders/pointGeometryShader.gs");
    this->updateShaderUniforms();

    glfwSetFramebufferSizeCallback(this->window, framebuffer_size_callback);
    glfwSetScrollCallback(this->window, scroll_callback);

    Wires wires;

    CreateVertexAction{std::make_shared<Vertex>(glm::vec2(5, 5), glm::vec3(0.5, 0.2, 0.5))}.Execute(&wires);
    CreateVertexAction{std::make_shared<Vertex>(glm::vec2(10, 5), glm::vec3(0.2, 0.5, 0.5))}.Execute(&wires);
    CreateWireAction{std::make_shared<Wire>((*wires.vertexMap.begin()).first, (*(++wires.vertexMap.begin())).first)}.Execute(&wires);

    CreateWireAction{wires.wireMap.begin()->first}.Rewind(&wires);
    CreateVertexAction{wires.getVertex(glm::vec2(10, 5))}.Rewind(&wires);
    CreateVertexAction{wires.getVertex(glm::vec2(5, 5))}.Rewind(&wires);

    WiresRenderer linesRenderer{&wires};
    linesRenderer.init();
    GridRenderer gridRenderer;
    gridRenderer.init();
    CursorRenderer cursorRenderer;
    cursorRenderer.init();

    Cursor cursor;

    glEnable(GL_PROGRAM_POINT_SIZE);

    bool dragging;
    glm::vec2 oldDragPos = glm::vec2(-1, -1);
    bool moving;
    Vertex* movingVertex;

    while(!glfwWindowShouldClose(this->window)) {
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

        cursor.update(this->getMousePos(), this->camera);

        std::shared_ptr<Vertex> hoveredVertex = wires.getVertex(cursor.hoveringCell);
        this->cursorProgram->setVec2("cursor", cursor.cursorPos, false);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        gridRenderer.draw(this->gridProgram);
        linesRenderer.draw(this->lineProgram, this->lineJointsProgram);
        if (!hoveredVertex) {
            cursorRenderer.draw(this->cursorProgram);
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
    this->lineProgram->setMat4("projection", projectionMat, true);
    this->lineJointsProgram->setMat4("projection", projectionMat, true);
    this->lineJointsProgram->setFloat("zoom", this->camera.zoom, false);
    this->gridProgram->setVec2("offset", this->camera.getPos(), true);
    this->gridProgram->setVec2("resolution", glm::vec2(windowWidth, windowHeight), false);
    this->gridProgram->setFloat("zoom", this->camera.zoom, false);
    this->cursorProgram->setMat4("projection", projectionMat, true);
    this->cursorProgram->setFloat("zoom", this->camera.zoom, false);
}

glm::vec2 Graphics::getMousePos() const {
    double x, y;
    glfwGetCursorPos(this->window, &x, &y);
    return {x, y};
}