//
// Created by felix on 8/2/24.
//

#include "wiresRenderer.h"
#include "graphics/util.h"


CablingRenderer::CablingRenderer(WireContainer *wireContainer, JointContainer* jointContainer) :
                                    wireContainer(wireContainer), jointContainer(jointContainer),
                                    jointVertexBuffer(GL_ARRAY_BUFFER, Util::getDefaultVertexLayout()),
                                    jointColorBuffer(GL_ARRAY_BUFFER, Util::getDefaultColorLayout()),
                                    wireVertexBuffer(GL_ARRAY_BUFFER, Util::getDefaultVertexLayout()),
                                    wireColorBuffer(GL_ARRAY_BUFFER, Util::getDefaultColorLayout()){
    this->jointVA = VertexArray{};
    this->jointVA.addBuffer(&this->jointVertexBuffer);
    this->jointVA.addBuffer(&this->jointColorBuffer);
    this->wireVA.addBuffer(&this->wireVertexBuffer);
    this->wireVA.addBuffer(&this->wireColorBuffer);
}

void CablingRenderer::drawWires(Program *shader) {
    if (this->wireVertexBuffer.getSize() != 0) {
        shader->use();
        this->wireVA.bind();
        glDrawArrays(GL_LINES, 0, this->wireVertexBuffer.getSize());
    }
}

void CablingRenderer::drawJoints(Program *shader) {
    if (this->jointVertexBuffer.getSize() != 0) {
        shader->use();
        this->jointVA.bind();
        glDrawArrays(GL_POINTS, 0, this->jointVertexBuffer.getSize());
    }
}

void CablingRenderer::render(Program *wireShader, Program *jointShader) {
    this->drawWires(wireShader);
    this->drawJoints(jointShader);
}

void CablingRenderer::fillJoints(std::set<const Joint *> &joints, std::vector<float> *vertexData, std::vector<unsigned char> *colorData) const {
    for (const auto &vertex: joints) {
    	Color color = vertex->network->getColor();
        vertexData->push_back(vertex->pos.x * 32);
        vertexData->push_back(vertex->pos.y * 32);
        colorData->push_back(color.x);
        colorData->push_back(color.y);
        colorData->push_back(color.z);
    }
}

void CablingRenderer::fillWires(std::set<const Wire*>& wires, std::vector<float> *vertexData, std::vector<unsigned char> *colorData) const {
    for (const auto &wire: wires) {
    	Color color = wire->network->getColor();
        vertexData->push_back(wire->start->pos.x * 32);
        vertexData->push_back(wire->start->pos.y * 32);
        vertexData->push_back(wire->end->pos.x * 32);
        vertexData->push_back(wire->end->pos.y * 32);
        colorData->push_back(color.x);
        colorData->push_back(color.y);
        colorData->push_back(color.z);
        colorData->push_back(color.x);
        colorData->push_back(color.y);
        colorData->push_back(color.z);
    }
}

void CablingRenderer::regenerateJoints(JointContainer *jointContainer) {
    std::set<const Joint*> joints = jointContainer->getJoints();
    this->regenerateJoints(joints);
}

void CablingRenderer::regenerateJoints(std::set<const Joint *> &joints) {
    this->jointVertexData.clear();
    this->jointColorData.clear();
    this->fillJoints(joints, &this->jointVertexData, &this->jointColorData);
    glBindBuffer(GL_ARRAY_BUFFER, this->vBOs[2]);
    glBufferData(GL_ARRAY_BUFFER, this->jointVertexData.size() * sizeof(float), this->jointVertexData.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, this->vBOs[3]);
    glBufferData(GL_ARRAY_BUFFER, this->jointColorData.size(), this->jointColorData.data(), GL_STATIC_DRAW);
}


void CablingRenderer::regenerateWires(WireContainer *wireContainer) {
    std::set<const Wire*> wires = wireContainer->getWires();
    this->regenerateWires(wires);
}

void CablingRenderer::regenerateWires(std::set<const Wire *> &wires) {
    this->wireVertexData.clear();
    this->wireColorData.clear();
    this->fillWires(wires, &this->wireVertexData, &this->wireColorData);
    glBindBuffer(GL_ARRAY_BUFFER, this->vBOs[0]);
    glBufferData(GL_ARRAY_BUFFER, this->wireVertexData.size() * sizeof(float), this->wireVertexData.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, this->vBOs[1]);
    glBufferData(GL_ARRAY_BUFFER, this->wireColorData.size(), this->wireColorData.data(), GL_STATIC_DRAW);
}


void CablingRenderer::regenerateData(JointContainer *jointContainer, WireContainer *wireContainer) {
    this->regenerateJoints(jointContainer);
    this->regenerateWires(wireContainer);
}

void CablingRenderer::regenerateData(std::set<const Joint *> &joints, std::set<const Wire *> &wires) {
    this->regenerateJoints(joints);
    this->regenerateWires(wires);
}

void CablingRenderer::moveJoint(size_t index, glm::vec2 newPos) {
    glBindBuffer(GL_ARRAY_BUFFER, this->vBOs[2]);
    float newPosData[2] = {newPos.x*32, newPos.y*32};
    glBufferSubData(GL_ARRAY_BUFFER, 2*sizeof(float)*index, 2*sizeof(float), newPosData);
}

void CablingRenderer::updateJointColor(size_t index, glm::vec3 newColor) {
    glBindBuffer(GL_ARRAY_BUFFER, this->vBOs[3]);
    unsigned char newColorData[3];
    newColorData[0] = newColor.x;
    newColorData[1] = newColor.y;
    newColorData[2] = newColor.z;
    glBufferSubData(GL_ARRAY_BUFFER, 3*index, 3, newColorData);
}

void CablingRenderer::moveWire(size_t index, glm::vec2 start, glm::vec2 end) {
    glBindBuffer(GL_ARRAY_BUFFER, this->vBOs[0]);
    float newPos[4] = {start.x*32, start.y*32, end.x*32, end.y*32};
    glBufferSubData(GL_ARRAY_BUFFER, 4*sizeof(float)*index, 4*sizeof(float), newPos);
}

void CablingRenderer::updateWireColor(size_t index, glm::vec3 newColor) {
    glBindBuffer(GL_ARRAY_BUFFER, this->vBOs[1]);
    unsigned char newColorData[6];
    newColorData[0] = newColor.x;
    newColorData[1] = newColor.y;
    newColorData[2] = newColor.z;
    newColorData[3] = newColor.x;
    newColorData[4] = newColor.y;
    newColorData[5] = newColor.z;
    glBufferSubData(GL_ARRAY_BUFFER, 6*index, 6, newColorData);
}

void CablingRenderer::updateNetwork(WireContainer *wireContainer, JointContainer* jointContainer, Network *network) {
	Color color = network->getColor();
    for (const auto &wire: network->wires) {
         const size_t index = wireContainer->getWireIndex(wire);
         this->updateWireColor(index, color);
    }
    for (const auto &joint: network->joints) {
        const size_t index = jointContainer->getJointIndex(joint);
        this->updateJointColor(index, color);
    }
}
