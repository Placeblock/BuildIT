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
    unsigned int vAOs[2] = {}; // Wire and Vertex VAOs
    unsigned int vBOs[4] = {}; // WiresRenderer, LineColors, Vertices, VertexColors,
    std::vector<float> vertexData;
    std::vector<float> vertexColorData;
    std::vector<float> wireVertexData;
    std::vector<float> wireColorData;
public:
    void init();

    void drawWires(Shader* shader);
    void drawVertices(Shader* shader);
    void render(Shader* wireShader, Shader* vertexShader);

    void fillVertices(std::set<std::shared_ptr<Vertex>>* vertices, std::vector<float>* vertexData, std::vector<float> *colorData) const;
    void fillWires(std::set<std::shared_ptr<Wire>>* wires, std::vector<float>* vertexData, std::vector<float> *colorData) const;
    void regenerateData(std::set<std::shared_ptr<Vertex>>* vertices, std::set<std::shared_ptr<Wire>>* wires);

    void updateVertexPos(int index, glm::vec2 newPos);
    void updateVertexColor(int index, glm::vec3 newColor);
    void updateWirePos(int index, glm::vec2 start, glm::vec2 end);
    void updateWireColor(int index, glm::vec3 newColor);

    void updateVertexPos(int index, std::shared_ptr<Vertex> vertex);
};

#endif //BUILDIT_WIRESRENDERER_H
