//
// Created by felix on 8/1/24.
//

#include "graphics.h"
#include <iostream>
#include "application.h"
#include "types.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    auto* graphics = static_cast<Graphics*>(glfwGetWindowUserPointer(window));
    graphics->application->onResize(intVec2(width, height));
}

void scroll_callback(GLFWwindow* window, double xOffset, double yOffset) {
    auto* graphics = static_cast<Graphics*>(glfwGetWindowUserPointer(window));
    graphics->application->onScroll(glm::vec2(xOffset, yOffset));
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
    auto* graphics = static_cast<Graphics*>(glfwGetWindowUserPointer(window));
    graphics->application->onMouseAction(button, action, mods);
}

void key_callback(GLFWwindow* window, int key, int scanCode, int action, int mods) {
    auto* graphics = static_cast<Graphics*>(glfwGetWindowUserPointer(window));
    graphics->application->onKeyAction(key, scanCode, action, mods);
}

void char_callback(GLFWwindow* window, unsigned int codepoint) {
    auto* graphics = static_cast<Graphics*>(glfwGetWindowUserPointer(window));
    graphics->application->onChar(codepoint);
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
    glfwSwapInterval(0);

    GLenum err = glewInit();
    if (GLEW_OK != err) {
        /* Problem: glewInit failed, something is seriously wrong. */
        printf("GLEW Error: %s\n", glewGetErrorString(err));
    }

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetKeyCallback(window, key_callback);
    glfwSetCharCallback(window, char_callback);

    glEnable(GL_PROGRAM_POINT_SIZE);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    this->application = new Application(this->simulation, window);
    this->renderer = application;

    double cursorX = 0, cursorY = 0;
    double previousTime = glfwGetTime();
    int frameCount = 0;
    while(!glfwWindowShouldClose(window)) {
        // FPS MEASUREMENTS
        double currentTime = glfwGetTime();
        frameCount++;
        if ( currentTime - previousTime >= 1.0 ) {
            std::cout << "FPS: " << frameCount << "\n";
            frameCount = 0;
            previousTime = currentTime;
        }

        // MOUSE MOVEMENT
        double newCursorX, newCursorY;
        glfwGetCursorPos(window, &newCursorX, &newCursorY);
        if (newCursorX != cursorX || newCursorY != cursorY) {
            this->application->onMouseMove(glm::vec2(newCursorX, newCursorY),
                                            glm::vec2(newCursorX-cursorX, newCursorY-cursorY));
            cursorX = newCursorX;
            cursorY = newCursorY;
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
