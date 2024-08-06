//
// Created by felix on 8/1/24.
//

#include "graphics.h"
#include <iostream>
#include "renderer/gridRenderer.h"
#include "application.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    auto* graphics = static_cast<Graphics*>(glfwGetWindowUserPointer(window));
    graphics->eventHandler->onResize(width, height);
}

void scroll_callback(GLFWwindow* window, double xOffset, double yOffset) {
    auto* graphics = static_cast<Graphics*>(glfwGetWindowUserPointer(window));
    graphics->eventHandler->onScroll(xOffset, yOffset);
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
    auto* graphics = static_cast<Graphics*>(glfwGetWindowUserPointer(window));
    graphics->eventHandler->onMouseAction(button, action, mods);
}

void key_callback(GLFWwindow* window, int key, int scanCode, int action, int mods) {
    auto* graphics = static_cast<Graphics*>(glfwGetWindowUserPointer(window));
    graphics->eventHandler->onKeyAction(key, scanCode, action, mods);
}

void Graphics::init() {
    glfwInit();

    GLFWwindow* window = createWindow();
    if (window == nullptr) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return;
    }
    glfwMakeContextCurrent(window);
    glfwSetWindowUserPointer(window, this);
    glViewport(0, 0, 640, 480);
    glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);
    //glfwSwapInterval(0);

    GLenum err = glewInit();
    if (GLEW_OK != err) {
        /* Problem: glewInit failed, something is seriously wrong. */
        printf("Error: %s\n", glewGetErrorString(err));
    }

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetKeyCallback(window, key_callback);

    glEnable(GL_PROGRAM_POINT_SIZE);

    const auto application = new Application(GraphicData(window, Camera()));
    this->eventHandler = application;
    this->renderer = application;

    double previousTime = glfwGetTime();
    int frameCount = 0;
    while(!glfwWindowShouldClose(window)) {
        double currentTime = glfwGetTime();
        frameCount++;
        if ( currentTime - previousTime >= 1.0 ) {
            std::cout << "FPS: " << frameCount << "\n";
            frameCount = 0;
            previousTime = currentTime;
        }

        this->renderer->render();

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

