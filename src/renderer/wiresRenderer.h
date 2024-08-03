//
// Created by felix on 8/2/24.
//

#ifndef BUILDIT_WIRESRENDERER_H
#define BUILDIT_WIRESRENDERER_H


#include <GL/glew.h>
#include <vector>
#include <map>
#include "shader.h"
#include "wires/wires.h"

class WiresRenderer {
private:
    Wires* wires;
    unsigned int vAOs[2] = {}; // Wire and Vertex VAOs
    unsigned int vBOs[4] = {}; // WiresRenderer, LineColors, Vertices, VertexColors,
    std::vector<float> vertices;
    std::vector<float> vertexColors;
    std::vector<float> wireVertices;
    std::vector<float> wireColors;
public:
    void init();
    void drawWires(Shader* shader);
    void drawVertices(Shader* shader);
    void draw(Shader* wireShader, Shader* vertexShader);
    void regenerateData();
    void updateVertexPos(std::shared_ptr<Vertex> vertex);
    WiresRenderer(Wires *wires);
};


#endif //BUILDIT_WIRESRENDERER_H
