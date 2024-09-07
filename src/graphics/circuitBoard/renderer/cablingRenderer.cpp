//
// Created by felix on 8/2/24.
//

#include "cablingRenderer.h"
#include "graphics/util.h"


CablingRenderer::CablingRenderer(Subject<JointAddEvent> *jointAddSubject, Subject<JointRemoveEvent> *jointRemoveSubject,
                                 Subject<WireAddEvent> *wireAddSubject, Subject<WireRemoveEvent> *wireRemoveSubject,
                                 Subject<NetworkAddEvent> *networkAddSubject, Subject<NetworkRemoveEvent> *networkRemoveSubject) :
                                 jointAddSubject(jointAddSubject), jointRemoveSubject(jointRemoveSubject),
                                 wireAddSubject(wireAddSubject), wireRemoveSubject(wireRemoveSubject),
                                 networkAddSubject(networkAddSubject), networkRemoveSubject(networkRemoveSubject),
                                 jointBuffer(GL_ARRAY_BUFFER, Util::getDefaultLayout()),
                                 wireBuffer(GL_ARRAY_BUFFER, Util::getDefaultLayout()) {
    this->jointVA.addBuffer(&this->jointBuffer);
    this->wireVA.addBuffer(&this->wireBuffer);
    this->jointAddSubject->subscribe(this);
    this->jointRemoveSubject->subscribe(this);
    this->wireAddSubject->subscribe(this);
    this->wireRemoveSubject->subscribe(this);
    this->networkAddSubject->subscribe(this);
    this->networkRemoveSubject->subscribe(this);
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

void CablingRenderer::updateJoint(Joint *joint) {
    this->jointBuffer.bind();
    unsigned int networkJointIndex=0;
    for (auto it = joint->network->joints.begin();
         it != joint->network->joints.end() && *it != joint; ++it, ++networkJointIndex) {}
    this->jointBuffer.updateElement({joint->getPos(), joint->network->getColor()},
                                    this->networkSections[joint->network].jointsSection, networkJointIndex);
}

void CablingRenderer::updateWire(Wire *wire) {
    this->wireBuffer.bind();
    Color color = wire->network->getColor();
    unsigned int networkWireIndex=0;
    for (auto it = wire->network->wires.begin();
         it != wire->network->wires.end() && *it != wire; ++it, ++networkWireIndex) {}
    this->wireBuffer.updateElement(VertexData{wire->start->getPos(), color},
                                   this->networkSections[wire->network].wiresSection, networkWireIndex*2);
    this->wireBuffer.updateElement(VertexData{wire->end->getPos(), color},
                                   this->networkSections[wire->network].wiresSection, networkWireIndex*2+1);
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

void CablingRenderer::update(const MoveEvent<Joint> &data, Joint *joint) {
    if (!data.before) {
        this->updateJoint(joint);
        for (const auto &wire: joint->wires) {
            this->updateWire(wire);
        }
    }
}

CablingRenderer::~CablingRenderer() {
    this->jointAddSubject->unsubscribe(this);
    this->jointRemoveSubject->unsubscribe(this);
    this->wireAddSubject->unsubscribe(this);
    this->wireRemoveSubject->unsubscribe(this);
    this->networkAddSubject->unsubscribe(this);
    this->networkRemoveSubject->unsubscribe(this);
}

void CablingRenderer::update(const NetworkAddEvent &data) {
    BufferSection *jointsSection = this->jointBuffer.createSection();
    BufferSection *wiresSection = this->wireBuffer.createSection();
    this->networkSections[data.network] = {jointsSection, wiresSection};
}

void CablingRenderer::update(const JointAddEvent &data) {
    this->jointBuffer.bind();
    VertexData element{data.joint->getPos(), data.joint->network->getColor()};
    BufferSection *jointSection = this->networkSections[data.joint->network].jointsSection;
    this->jointBuffer.addElement(element, jointSection);
}

void CablingRenderer::update(const JointRemoveEvent &data) {
    this->jointBuffer.bind();
    BufferSection *jointSection = this->networkSections[data.joint->network].jointsSection;
    unsigned int networkJointIndex=0;
    for (auto it = data.joint->network->joints.begin();
         it != data.joint->network->joints.end() && *it != data.joint; ++it, ++networkJointIndex) {}
    this->jointBuffer.removeElement(jointSection, networkJointIndex);
}

void CablingRenderer::update(const WireAddEvent &data) {
    this->wireBuffer.bind();
    Color color = data.wire->network->getColor();
    VertexData startElement{data.wire->start->getPos(), color};
    VertexData endElement{data.wire->end->getPos(), color};
    BufferSection *wiresSection = this->networkSections[data.wire->network].wiresSection;
    this->wireBuffer.addElement(startElement, wiresSection);
    this->wireBuffer.addElement(endElement, wiresSection);
}

void CablingRenderer::update(const WireRemoveEvent &data) {
    this->wireBuffer.bind();
    BufferSection *wiresSection = this->networkSections[data.wire->network].wiresSection;
    unsigned int networkWireIndex=0;
    for (auto it = data.wire->network->wires.begin();
         it != data.wire->network->wires.end() && *it != data.wire; ++it, ++networkWireIndex) {}
    this->wireBuffer.removeElement(wiresSection, networkWireIndex*2);
    this->wireBuffer.removeElement(wiresSection, networkWireIndex*2+1);
}

void CablingRenderer::update(const NetworkRemoveEvent &data) {
    this->wireBuffer.removeSection(this->networkSections[data.network].wiresSection);
    this->jointBuffer.removeSection(this->networkSections[data.network].jointsSection);
    this->networkSections.erase(data.network);
}
