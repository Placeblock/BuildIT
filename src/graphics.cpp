//
// Created by felix on 8/1/24.
//

#include "graphics.h"
#include <iostream>
#include "shader.h"

#include <glm/gtc/matrix_transform.hpp>

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    auto* graphics = static_cast<Graphics*>(glfwGetWindowUserPointer(window));
    glViewport(0, 0, width, height);
    // Update the projection-matrix to fill the whole screen
    glm::mat4 projectionMat = graphics->camera.getProjectionMat(glm::vec2(width, height));
    graphics->lineProgram->setMat4("projection", projectionMat, true);
    graphics->lineJointsProgram->setMat4("projection", projectionMat, true);
}

void Graphics::init() {
    glfwInit();

    GLFWwindow *window = createWindow();
    if (window == nullptr) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return;
    }
    glfwMakeContextCurrent(window);
    glfwSetWindowUserPointer(window, this);
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
    glm::mat4 projectionMat = this->camera.getProjectionMat(this->initScreenSize);
    this->lineProgram->setMat4("projection", projectionMat, true);
    this->lineJointsProgram->setMat4("projection", projectionMat, true);

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    unsigned int VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    unsigned int VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    float vertices[] = {
            30, 30, 300, 30, 300, 300,
            30, 100, 130, 200, 230, 200
    };
    GLint indices[] = {0, 3};
    GLint sizes[] = {3, 3};
    std::cout << (sizeof(sizes)/sizeof(int)) << "\n";
    std::cout << (sizeof(indices)/sizeof(int)) << "\n";
    std::cout << (sizeof(vertices)/sizeof(float)) << "\n";
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)nullptr);
    glEnableVertexAttribArray(0);

    glBindVertexArray(VAO);

    glEnable(GL_PROGRAM_POINT_SIZE);

    bool dragging = false;

    while(!glfwWindowShouldClose(window)) {
        int state = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT);
        if (state == GLFW_PRESS) {
            upgrade_cow();
        }

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        this->lineProgram->use();
        glMultiDrawArrays(GL_LINE_STRIP, indices, sizes, 2);
        this->lineJointsProgram->use();
        glDrawArrays(GL_POINTS, 0, 6);

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
    GLFWwindow* window = glfwCreateWindow(640, 480, "BuildIT", nullptr, nullptr);
    return window;
}

glm::mat4 Camera::getProjectionMat(glm::vec2 screenSize) {
    const glm::vec2 pos = this->getPos();
    return glm::ortho(pos.x, pos.x+screenSize.x*this->zoom, pos.y+screenSize.y*this->zoom, pos.y);
}

glm::vec2 Camera::screenToWorld(glm::vec2 screenPos) {
    return this->getPos() + this->zoom*screenPos;
}

glm::vec2 Camera::getPos() {
    return this->target-this->offset;
}
