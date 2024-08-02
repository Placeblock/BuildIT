//
// Created by felix on 8/1/24.
//

#include "graphics.h"
#include <iostream>
#include "shader.h"
#include "Lines.h"
#include "grid.h"
#include "cursor.h"

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

    GLenum err = glewInit();
    if (GLEW_OK != err) {
        /* Problem: glewInit failed, something is seriously wrong. */
        printf("Error: %s\n", glewGetErrorString(err));
    }
    printf("Status: Using GLEW %s\n", glewGetString(GLEW_VERSION));

    this->lineJointsProgram = new Shader("resources/shaders/defaultVertexShader.vs",
                                   "resources/shaders/pointFragmentShader.fs",
                                   "resources/shaders/pointGeometryShader.gs");
    this->lineProgram = new Shader("resources/shaders/defaultVertexShader.vs",
                                   "resources/shaders/defaultFragmentShader.fs",
                                   "resources/shaders/lineGeometryShader.gs");
    this->gridProgram = new Shader("resources/shaders/defaultVertexShader.vs",
                                   "resources/shaders/gridShader.fs");
    this->cursorProgram = new Shader("resources/shaders/cursorVertexShader.vs",
                                   "resources/shaders/pointFragmentShader.fs",
                                     "resources/shaders/pointGeometryShader.gs");
    this->updateShaderUniforms();

    glfwSetFramebufferSizeCallback(this->window, framebuffer_size_callback);
    glfwSetScrollCallback(this->window, scroll_callback);

    Lines lines;
    lines.init();

    Grid grid;
    grid.init();

    Cursor cursor;
    cursor.init();

    glEnable(GL_PROGRAM_POINT_SIZE);

    bool dragging;
    glm::vec2 oldDragPos = glm::vec2(-1, -1);
    glm::vec2 hoveringCell;
    glm::vec2 cursorPos;

    while(!glfwWindowShouldClose(this->window)) {
        int state = glfwGetMouseButton(this->window, GLFW_MOUSE_BUTTON_LEFT);
        dragging = state == GLFW_PRESS;
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

        glm::vec2 mousePos = this->getMousePos();
        glm::vec2 gridMousePos = this->camera.screenToWorld(mousePos) / 32.0f;
        glm::vec2 roundedGridMousePos = glm::round(gridMousePos);
        glm::vec2 deltaNearestCell = gridMousePos - roundedGridMousePos;
        if (glm::length(deltaNearestCell) < 0.4 || glm::length(hoveringCell-gridMousePos) > 1.5) {
            hoveringCell = roundedGridMousePos;
        }
        glm::vec2 deltaHoveringCell = gridMousePos - hoveringCell;
        gridMousePos = hoveringCell * 32.0f + deltaHoveringCell * 15.0f;
        cursorPos += (gridMousePos-cursorPos)*0.5f;

        bool hoveringVertex = false;
        for (const auto &network: lines.networks) {
            for (const auto &vertex: network.vertices) {
                if (vertex.cell == hoveringCell) {
                    hoveringVertex = true;
                }
            }
        }
        this->cursorProgram->setVec2("cursor", cursorPos, false);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        grid.draw(this->gridProgram);
        lines.draw(this->lineProgram, this->lineJointsProgram);
        if (!hoveringVertex) {
            cursor.draw(this->cursorProgram);
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

glm::mat4 Camera::getProjectionMat(glm::vec2 screenSize) {
    const glm::vec2 pos = this->getPos();
    return glm::ortho(pos.x, pos.x+screenSize.x*this->getZoomScalar(), pos.y+screenSize.y*this->getZoomScalar(), pos.y);
}

glm::vec2 Camera::screenToWorld(glm::vec2 screenPos) {
    return this->getPos() + this->getZoomScalar()*screenPos;
}

glm::vec2 Camera::getPos() {
    return this->target+this->offset*this->getZoomScalar();
}

float Camera::getZoomScalar() const {
    if (this->zoom == 0) return 0;
    return 1/this->zoom;
}
