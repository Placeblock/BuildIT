//
// Created by felix on 8/2/24.
//

#include "cablingRenderer.h"
#include "graphics/util.h"


CablingRenderer::CablingRenderer() :
                                 jointBuffer(GL_ARRAY_BUFFER, Util::getDefaultLayout()),
                                 wireBuffer(GL_ARRAY_BUFFER, Util::getDefaultLayout()) {
    this->jointVA.addBuffer(&this->jointBuffer);
    this->wireVA.addBuffer(&this->wireBuffer);
}

void CablingRenderer::drawWires(Program *shader) {
    if (this->wireBuffer.size() != 0) {
        shader->use();
        this->wireVA.bind();
        glDrawArrays(GL_LINES, 0, this->wireBuffer.size());
    }
}

void CablingRenderer::drawJoints(Program *shader) {
    if (this->jointBuffer.size() != 0) {
        shader->use();
        this->jointVA.bind();
        glDrawArrays(GL_POINTS, 0, this->jointBuffer.size());
    }
}

void CablingRenderer::render(Program *wireShader, Program *jointShader) {
    this->drawWires(wireShader);
    this->drawJoints(jointShader);
}

void CablingRenderer::updateJoint(Joint *joint, glm::vec2 newPos) {
    this->jointBuffer.bind();
    unsigned int networkJointIndex=0;
    for (auto it = joint->getNetwork()->joints.begin();
         it != joint->getNetwork()->joints.end() && *it != joint; ++it, ++networkJointIndex) {}
    this->jointBuffer.updateElement({newPos, joint->getNetwork()->getColor()},
                                    this->networkSections[joint->getNetwork()].jointsSection, networkJointIndex);
}

void CablingRenderer::updateWire(Wire *wire, glm::vec2 pos, bool start) {
    this->wireBuffer.bind();
    Color color = wire->getNetwork()->getColor();
    unsigned int networkWireIndex=0;
    for (auto it = wire->getNetwork()->wires.begin();
         it != wire->getNetwork()->wires.end() && *it != wire; ++it, ++networkWireIndex) {}
    unsigned int sectionIndex = networkWireIndex*2 + (start ? 0 : 1);
    NetworkSection section = this->networkSections[wire->getNetwork()];
    this->wireBuffer.updateElement(VertexData{pos, color}, section.wiresSection, sectionIndex);
}

void CablingRenderer::updateNetwork(Network *network) {
	Color color = network->getColor();
    this->wireBuffer.bind();
    std::vector<VertexData> wireData;
    for (const auto &wire: network->wires) {
        wireData.emplace_back(wire->start->getPos(), color);
        wireData.emplace_back(wire->end->getPos(), color);
    }
    this->wireBuffer.updateSection(this->networkSections[network].wiresSection, wireData);
    this->jointBuffer.bind();
    std::vector<VertexData> jointData;
    for (const auto &joint: network->joints) {
        jointData.emplace_back(joint->getPos(), color);
    }
    this->jointBuffer.updateSection(this->networkSections[network].jointsSection, jointData);
}

void CablingRenderer::addJoint(Joint *joint) {
    this->jointBuffer.bind();
    VertexData element{joint->getPos(), joint->getNetwork()->getColor()};
    BufferSection *jointSection = this->networkSections[joint->getNetwork()].jointsSection;
    this->jointBuffer.addElement(element, jointSection);
    joint->Subject<MoveEvent>::subscribe(this);
    joint->Subject<NetworkChangeEvent>::subscribe(this);
}

void CablingRenderer::removeJoint(Joint *joint) {
    this->jointBuffer.bind();
    BufferSection *jointSection = this->networkSections[joint->getNetwork()].jointsSection;
    unsigned int networkJointIndex=0;
    for (auto it = joint->getNetwork()->joints.begin();
         it != joint->getNetwork()->joints.end() && *it != joint; ++it, ++networkJointIndex) {}
    this->jointBuffer.removeElement(jointSection, networkJointIndex);
    joint->Subject<MoveEvent>::unsubscribe(this);
}

void CablingRenderer::addWire(Wire *wire) {
    this->wireBuffer.bind();
    Color color = wire->getNetwork()->getColor();
    VertexData startElement{wire->start->getPos(), color};
    VertexData endElement{wire->end->getPos(), color};
    BufferSection *wiresSection = this->networkSections[wire->getNetwork()].wiresSection;
    this->wireBuffer.addElement(startElement, wiresSection);
    this->wireBuffer.addElement(endElement, wiresSection);
    wire->Subject<NetworkChangeEvent>::subscribe(this);
}

void CablingRenderer::removeWire(Wire *wire) {
    this->wireBuffer.bind();
    BufferSection *wiresSection = this->networkSections[wire->getNetwork()].wiresSection;
    unsigned int networkWireIndex=0;
    for (auto it = wire->getNetwork()->wires.begin();
         it != wire->getNetwork()->wires.end() && *it != wire; ++it, ++networkWireIndex) {}
    this->wireBuffer.removeElement(wiresSection, networkWireIndex*2);
    this->wireBuffer.removeElement(wiresSection, networkWireIndex*2+1);
}

void CablingRenderer::notify(const MoveEvent& data) {
    if (Joint *joint = dynamic_cast<Joint*>(data.movable)) {
        if (data.before) return;
        this->updateJoint(joint, data.newPos);
        for (const auto &wire: joint->wires) {
            this->updateWire(wire, data.newPos, wire->start == joint);
        }
    }
}

void CablingRenderer::notify(const NetworkChangeEvent &data) {
    if (data.before) {
        if (Joint *joint = dynamic_cast<Joint *>(data.networkable)) {
            this->removeJoint(joint);
        } else if (Wire *wire = dynamic_cast<Wire *>(data.networkable)) {
            this->removeWire(wire);
        }
    } else if (this->networkSections.contains(data.newNetwork)) {
        if (Joint *joint = dynamic_cast<Joint *>(data.networkable)) {
            this->addJoint(joint);
        } else if (Wire *wire = dynamic_cast<Wire *>(data.networkable)) {
            this->addWire(wire);
        }
    }
    this->jointBuffer.bufferAll();
    this->wireBuffer.bufferAll();
}

void CablingRenderer::addNetwork(Network *network) {
    if (this->networkSections.contains(network)) return;
    BufferSection *jointsSection = this->jointBuffer.createSection();
    BufferSection *wiresSection = this->wireBuffer.createSection();
    this->networkSections[network] = {jointsSection, wiresSection};
    for (const auto &joint: network->joints) {
        this->addJoint(joint);
        joint->Movable::subscribe(this);
    }
    for (const auto &wire: network->wires) {
        this->addWire(wire);
    }
    this->jointBuffer.bufferAll();
    this->wireBuffer.bufferAll();
}

void CablingRenderer::removeNetwork(Network *network) {
    if (!this->networkSections.contains(network)) return;
    NetworkSection section = this->networkSections[network];
    this->jointBuffer.removeSection(section.jointsSection);
    this->wireBuffer.removeSection(section.wiresSection);
    this->networkSections.erase(network);
    for (const auto &joint: network->joints) {
        joint->Movable::unsubscribe(this);
    }
    this->jointBuffer.bufferAll();
    this->wireBuffer.bufferAll();
}
