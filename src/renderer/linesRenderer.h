//
// Created by felix on 8/2/24.
//

#ifndef BUILDIT_LINESRENDERER_H
#define BUILDIT_LINESRENDERER_H


#include <GL/glew.h>
#include <vector>
#include <map>
#include "shader.h"

class Vertex {
public:
    glm::vec2 cell;
    glm::vec3 color;
};

class Cable {
public:
    Vertex* start;
    Vertex* end;
    glm::vec3 color;
};

class Network {
public:
    std::vector<Vertex*> vertices;
    std::vector<Cable> cables;
    void fillVertices(std::vector<float>* array) const;
    void fillVertexColors(std::vector<float>* array) const;
    void fillLines(std::vector<float>* array) const;
    void fillLineColors(std::vector<float>* array) const;
};

class LinesRenderer {
private:
    unsigned int vAOs[2] = {}; // Cable and Vertex VAOs
    unsigned int vBOs[4] = {}; // LinesRenderer, LineColors, Vertices, VertexColors,
    std::map<glm::vec2, Network*> vertexPositions;
    std::vector<float> vertices;
    std::vector<float> vertexColors;
    std::vector<float> lines;
    std::vector<float> lineColors;
public:
    std::vector<Network*> networks;
    void init();
    void drawLines(Shader* shader);
    void drawVertices(Shader* shader);
    void draw(Shader* lineShader, Shader* vertexShader);
    void regenerateData();
};


#endif //BUILDIT_LINESRENDERER_H
