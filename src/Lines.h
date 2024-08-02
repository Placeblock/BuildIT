//
// Created by felix on 8/2/24.
//

#ifndef BUILDIT_LINES_H
#define BUILDIT_LINES_H


#include <GL/glew.h>
#include <vector>
#include <map>
#include "shader.h"

class Vertex {
public:
    glm::vec2 pos;
    glm::vec3 color;
};

class Line {
public:
    glm::vec3 color;
    std::vector<Vertex> vertices;
    void fillVertices(std::vector<float>*) const;
    void fillVertexColors(std::vector<float>*) const;
    void fillLineColor(std::vector<float>*) const;
    void setVertexColor(int i, glm::vec3 color);
    void setVertexColor(glm::vec2 pos, glm::vec3 color);
};

class Lines {
private:
    unsigned int vAO;
    unsigned int vBOs[3] = {};
    std::vector<Line> lines;
    std::map<glm::vec2, Line*> vertexPositions;
    std::vector<float> vertices;
    std::vector<float> vertexColors;
    std::vector<float> lineColors;
    std::vector<GLint> indices;
    std::vector<GLint> sizes;
    /*float vertexPositions[12] = {
            0, 0, 320, 0, 320, 320,
            0, 96, 96, 192, 256, 192
    };
    float vertexColors[18] = {
            0.87, 0.2, 0.16, 0.87, 0.2, 0.16,
            0.87, 0.2, 0.16, 0.87, 0.2, 0.16,
            0.87, 0.2, 0.16, 0.87, 0.2, 0.16
    };
    GLint indices[2] = {0, 3};
    GLint sizes[2] = {3, 3};*/
public:
    void init();
    void drawLines(Shader* shader);
    void drawVertices(Shader* shader);
    void draw(Shader* lineShader, Shader* vertexShader);
    void regenerateData();
};


#endif //BUILDIT_LINES_H
