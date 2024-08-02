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
    graphics->projectionMat = glm::ortho(0.0f, float(width), float(height), 0.0f, -1.0f, 1.0f);
    graphics->shader->setMat4("projection", graphics->projectionMat);
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
    glfwSwapInterval(0);
    glfwSetWindowUserPointer(window, this);
    glViewport(0, 0, 640, 480);

    GLenum err = glewInit();
    if (GLEW_OK != err) {
        /* Problem: glewInit failed, something is seriously wrong. */
        printf("Error: %s\n", glewGetErrorString(err));
    }
    printf("Status: Using GLEW %s\n", glewGetString(GLEW_VERSION));

    this->shader = new Shader("resources/shaders/defaultVertexShader.vs",
                  "resources/shaders/defaultFragmentShader.fs");
    shader->use();
    shader->setMat4("projection", this->projectionMat);

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    unsigned int VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    unsigned int VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    float vertices[600000] = {};
    GLint indices[100000] = {};
    GLint sizes[100000] = {};
    for (int i = 0; i < 100000; ++i) {
        for (int j = 0; j < 3; ++j) {
            const float x = float(rand()) / float(RAND_MAX) * 300.0f;
            const float y = float(rand()) / float(RAND_MAX) * 300.0f;
            vertices[i*3+j*2] = x;
            vertices[i*3+j*2+1] = y;
        }
        indices[i] = i*3;
        sizes[i] = 3;
    }
    std::cout << (sizeof(sizes)/sizeof(int)) << "\n";
    std::cout << (sizeof(indices)/sizeof(int)) << "\n";
    std::cout << (sizeof(vertices)/sizeof(float)) << "\n";
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)nullptr);
    glEnableVertexAttribArray(0);

    glPrimitiveRestartIndex(0xFFFF);

    glBindVertexArray(VAO);

    double previousTime = glfwGetTime();
    int frameCount = 0;
    while(!glfwWindowShouldClose(window)) {
        frameCount++;
        double currentTime = glfwGetTime();
        if ( currentTime - previousTime >= 1.0 ) {
            std::cout << "FPS: " << frameCount << "\n";
            frameCount = 0;
            previousTime = currentTime;
        }
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        glMultiDrawArrays(GL_LINE_STRIP, indices, sizes, 100000);

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