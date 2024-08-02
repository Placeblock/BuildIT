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
    glm::vec2 cell;
    glm::vec3 color;
};

class Line {
public:
    Vertex* start;
    Vertex* end;
    glm::vec3 color;
};

class Network {
public:
    std::vector<Vertex> vertices;
    std::vector<Line> lines;
    bool isOnLine(glm::vec2 cell);
    void fillVertices(std::vector<float>* array) const;
    void fillVertexColors(std::vector<float>* array) const;
    void fillLines(std::vector<float>* array) const;
    void fillLineColors(std::vector<float>* array) const;
};

class Lines {
private:
    unsigned int vAOs[2] = {}; // Line and Vertex VAOs
    unsigned int vBOs[4] = {}; // Lines, LineColors, Vertices, VertexColors,
    std::vector<Network> networks;
    std::map<glm::vec2, Line*> vertexPositions;
    std::vector<float> vertices;
    std::vector<float> vertexColors;
    std::vector<float> lines;
    std::vector<float> lineColors;
public:
    void init();
    void drawLines(Shader* shader);
    void drawVertices(Shader* shader);
    void draw(Shader* lineShader, Shader* vertexShader);
    void regenerateData();
};


#endif //BUILDIT_LINES_H
