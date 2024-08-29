//
// Created by felix on 8/2/24.
//

#ifndef BUILDIT_WIRESRENDERER_H
#define BUILDIT_WIRESRENDERER_H


#include <GL/glew.h>
#include <vector>
#include <map>
#include "graphics/data/program.h"
#include "graphics/circuitBoard/components/wires/jointContainer.h"
#include "graphics/circuitBoard/components/wires/wireContainer.h"

class WiresRenderer {
private:
    unsigned int vAOs[2] = {}; // Wire and Joint VAOs
    unsigned int vBOs[4] = {}; // WiresRenderer, LineColors, Vertices, VertexColors,
    std::vector<float> jointVertexData;
    std::vector<unsigned char> jointColorData;
    std::vector<float> wireVertexData;
    std::vector<unsigned char> wireColorData;
public:
    WiresRenderer();

    void drawWires(Program* shader);
    void drawJoints(Program* shader);
    void render(Program* wireShader, Program* jointShader);

    void fillJoints(std::set<const Joint*>& joints, std::vector<float>* vertexData, std::vector<unsigned char> *colorData) const;
    void fillWires(std::set<const Wire*>& wires, std::vector<float>* vertexData, std::vector<unsigned char> *colorData) const;
    void regenerateJoints(JointContainer *jointContainer);
    void regenerateJoints(std::set<const Joint*>& joints);
    void regenerateWires(WireContainer *wireContainer);
    void regenerateWires(std::set<const Wire*>& wires);
    void regenerateData(JointContainer *jointContainer, WireContainer *wireContainer);
    void regenerateData(std::set<const Joint*>& joints, std::set<const Wire*>& wires);

    void moveJoint(size_t index, glm::vec2 newPos);
    void updateJointColor(size_t index, glm::vec3 newColor);
    void moveWire(size_t index, glm::vec2 start, glm::vec2 end);
    void updateWireColor(size_t index, glm::vec3 newColor);

    void updateNetwork(WireContainer *wireContainer, JointContainer* jointContainer, Network *network);
};

#endif //BUILDIT_WIRESRENDERER_H
