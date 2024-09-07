//
// Created by felix on 8/2/24.
//

#include "cablingRenderer.h"
#include "graphics/util.h"


CablingRenderer::CablingRenderer(TypedSubject<ComponentAddEvent, Joint> *jointAddSubject, TypedSubject<ComponentRemoveEvent, Joint> *jointRemoveSubject,
                                 Subject<WireAddEvent> *wireAddSubject, Subject<WireRemoveEvent> *wireRemoveSubject,
                                 Subject<NetworkAddEvent> *networkAddSubject, Subject<NetworkRemoveEvent> *networkRemoveSubject) :
                                 jointBuffer(GL_ARRAY_BUFFER, Util::getDefaultLayout()),
                                 wireBuffer(GL_ARRAY_BUFFER, Util::getDefaultLayout()) {
    this->jointVA.addBuffer(&this->jointBuffer);
    this->wireVA.addBuffer(&this->wireBuffer);

    jointAddSubject->subscribe(this);
    jointRemoveSubject->subscribe(this);
    wireAddSubject->subscribe(this);
    wireRemoveSubject->subscribe(this);
    networkAddSubject->subscribe(this);
    networkRemoveSubject->subscribe(this);
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
    for (auto it = joint->getNetwork()->joints.begin();
         it != joint->getNetwork()->joints.end() && *it != joint; ++it, ++networkJointIndex) {}
    this->jointBuffer.updateElement({joint->getPos(), joint->getNetwork()->getColor()},
                                    this->networkSections[joint->getNetwork()].jointsSection, networkJointIndex);
}

void CablingRenderer::updateWire(Wire *wire) {
    this->wireBuffer.bind();
    Color color = wire->getNetwork()->getColor();
    unsigned int networkWireIndex=0;
    for (auto it = wire->getNetwork()->wires.begin();
         it != wire->getNetwork()->wires.end() && *it != wire; ++it, ++networkWireIndex) {}
    this->wireBuffer.updateElement(VertexData{wire->start->getPos(), color},
                                   this->networkSections[wire->getNetwork()].wiresSection, networkWireIndex*2);
    this->wireBuffer.updateElement(VertexData{wire->end->getPos(), color},
                                   this->networkSections[wire->getNetwork()].wiresSection, networkWireIndex*2+1);
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


void CablingRenderer::notify(Joint *joint, const MoveEvent& data) {
    if (!data.before) {
        this->updateJoint(joint);
        for (const auto &wire: joint->wires) {
            this->updateWire(wire);
        }
    }
}

void CablingRenderer::notify(TypedSubject<ComponentAddEvent, Joint> *subject, const ComponentAddEvent& data) {
    this->addJoint(static_cast<Joint*>(data.component));
}

void CablingRenderer::notify(TypedSubject<ComponentRemoveEvent, Joint> *subject, const ComponentRemoveEvent& data) {
    this->removeJoint(static_cast<Joint*>(data.component));
}

void CablingRenderer::notify(Subject<WireAddEvent> *subject, const WireAddEvent &data) {
    this->addWire(data.wire);
}

void CablingRenderer::notify(Subject<WireRemoveEvent> *subject, const WireRemoveEvent &data) {
    this->removeWire(data.wire);
}

void CablingRenderer::notify(Subject<NetworkAddEvent> *subject, const NetworkAddEvent &data) {
    for (const auto &joint: data.network->joints) {
        this->addJoint(joint);
    }
    for (const auto &wire: data.network->wires) {
        this->addWire(wire);
    }
    BufferSection *jointsSection = this->jointBuffer.createSection();
    BufferSection *wiresSection = this->wireBuffer.createSection();
    this->networkSections[data.network] = {jointsSection, wiresSection};
}

void CablingRenderer::notify(Subject<NetworkRemoveEvent> *subject, const NetworkRemoveEvent &data) {
    for (const auto &joint: data.network->joints) {
        this->removeJoint(joint);
    }
    for (const auto &wire: data.network->wires) {
        this->removeWire(wire);
    }
    this->wireBuffer.removeSection(this->networkSections[data.network].wiresSection);
    this->jointBuffer.removeSection(this->networkSections[data.network].jointsSection);
    this->networkSections.erase(data.network);
}

void CablingRenderer::notify(Joint *joint, const NetworkChangeEvent &data) {
    if (data.before) {
        this->removeJoint(joint);
    } else {
        this->addJoint(joint);
    }
}

void CablingRenderer::notify(Wire *wire, const NetworkChangeEvent &data) {
    if (data.before) {
        this->removeWire(wire);
    } else {
        this->addWire(wire);
    }
}