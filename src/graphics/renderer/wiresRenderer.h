//
// Created by felix on 8/2/24.
//

#ifndef BUILDIT_WIRESRENDERER_H
#define BUILDIT_WIRESRENDERER_H


#include <GL/glew.h>
#include <vector>
#include <map>
#include "graphics/data/program.h"
#include "graphics/elements/wires/wires.h"

class WiresRenderer {
private:
    unsigned int vAOs[2] = {}; // Wire and Vertex VAOs
    unsigned int vBOs[4] = {}; // WiresRenderer, LineColors, Vertices, VertexColors,
    std::vector<float> vertexData;
    std::vector<unsigned char> vertexColorData;
    std::vector<float> wireVertexData;
    std::vector<unsigned char> wireColorData;
public:
    WiresRenderer();

    void drawWires(Program* shader);
    void drawVertices(Program* shader);
    void render(Program* wireShader, Program* vertexShader);

    void fillVertices(std::set<std::shared_ptr<Vertex>>* vertices, std::vector<float>* vertexData, std::vector<unsigned char> *colorData) const;
    void fillWires(std::set<std::shared_ptr<Wire>>* wires, std::vector<float>* vertexData, std::vector<unsigned char> *colorData) const;
    void regenerateData(std::set<std::shared_ptr<Vertex>>* vertices, std::set<std::shared_ptr<Wire>>* wires);

    void updateVertexPos(int index, glm::vec2 newPos);
    void updateVertexColor(int index, glm::vec3 newColor);
    void updateWirePos(int index, glm::vec2 start, glm::vec2 end);
    void updateWireColor(int index, glm::vec3 newColor);
};

#endif //BUILDIT_WIRESRENDERER_H
