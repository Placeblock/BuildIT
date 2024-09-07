//
// Created by felix on 8/2/24.
//

#ifndef BUILDIT_WIRESRENDERER_H
#define BUILDIT_WIRESRENDERER_H


#include <GL/glew.h>
#include <vector>
#include <map>
#include "graphics/data/program.h"
#include "graphics/circuitBoard/components/cabling/jointContainer.h"
#include "graphics/circuitBoard/components/cabling/wireContainer.h"
#include "graphics/buffer/vertexBuffer.h"
#include "graphics/buffer/vertexArray.h"

struct VertexData {
    glm::vec2 pos;
    Color color;
};

struct NetworkSection {
    BufferSection *jointsSection;
    BufferSection *wiresSection;
};

class CablingRenderer {
private:
    WireContainer *wireContainer;
    JointContainer* jointContainer;

    VertexArray jointVA;
    SectionedIndexedBuffer<VertexData> jointBuffer;
    VertexArray wireVA;
    SectionedIndexedBuffer<VertexData> wireVertexBuffer;
    std::unordered_map<Network*, NetworkSection> networkSections;
public:
    CablingRenderer(WireContainer *wireContainer, JointContainer* jointContainer);

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
