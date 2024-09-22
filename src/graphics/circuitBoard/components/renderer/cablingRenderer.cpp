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

void CablingRenderer::drawWires(const Program *shader) {
    if (this->wireBuffer.size() != 0) {
        shader->use();
        this->wireVA.bind();
        glDrawArrays(GL_LINES, 0, this->wireBuffer.size());
    }
}

void CablingRenderer::drawJoints(const Program *shader) {
    if (this->jointBuffer.size() != 0) {
        shader->use();
        this->jointVA.bind();
        glDrawArrays(GL_POINTS, 0, this->jointBuffer.size());
    }
}

void CablingRenderer::render(const Program *wireShader, const Program *jointShader) {
    this->drawWires(wireShader);
    this->drawJoints(jointShader);
}

void CablingRenderer::updateJoint(const Joint *joint, const glm::vec2 newPos) {
    this->jointBuffer.bind();
    unsigned int networkJointIndex=0;
    auto&[section, joints] = this->jointsSections[joint->getNetwork()];
    for (auto it = joints.begin(); it != joints.end() && *it != joint; ++it, ++networkJointIndex) {}
    this->jointBuffer.updateElement({newPos, joint->getNetwork()->getColor()},
                                    section, networkJointIndex);
}

void CablingRenderer::updateWire(const Wire *wire, const glm::vec2 pos, const bool start) {
    if (!this->wiresSections.contains(wire->getNetwork())) {
        return;
    }
    this->wireBuffer.bind();
    const Color color = wire->getNetwork()->getColor();
    unsigned int networkWireIndex=0;
    auto&[_, wires] = this->wiresSections[wire->getNetwork()];
    for (auto it = wires.begin(); it != wires.end() && *it != wire; ++it, ++networkWireIndex) {}
    const unsigned int sectionIndex = networkWireIndex*2 + (start ? 0 : 1);
    this->wireBuffer.updateElement(VertexData{pos, color}, this->wiresSections[wire->getNetwork()].section, sectionIndex);
}

void CablingRenderer::updateNetwork(Network *network) {
	Color color = network->getColor();
    if (this->wiresSections.contains(network)) {
        this->wireBuffer.bind();
        std::vector<VertexData> wireData;
        for (const auto &wire: this->wiresSections[network].wires) {
            wireData.emplace_back(wire->start->getPos(), color);
            wireData.emplace_back(wire->end->getPos(), color);
        }
        this->wireBuffer.updateSection(this->wiresSections[network].section, wireData);
    }
    if (this->jointsSections.contains(network)) {
        this->jointBuffer.bind();
        std::vector<VertexData> jointData;
        for (const auto &joint: this->jointsSections[network].joints) {
            jointData.emplace_back(joint->getPos(), color);
        }
        this->jointBuffer.updateSection(this->jointsSections[network].section, jointData);
    }
}

void CablingRenderer::addJoint(Joint *joint, const bool subscribe) {
    auto&[section, joints] = this->jointsSections[joint->getNetwork()];
    if (section == nullptr) {
        section = this->jointBuffer.createSection();
    } else if (std::ranges::find(joints, joint) != joints.end()) return;
    const VertexData element{joint->getPos(), joint->getNetwork()->getColor()};
    this->jointBuffer.addElement(element, section);
    joints.push_back(joint);
    if (subscribe) {
        joint->Movable::subscribe(this);
        joint->Networkable::subscribe(this);
    }
    this->jointBuffer.bufferAll();
    joint->getNetwork()->subscribe(this);
}

void CablingRenderer::removeJoint(Joint *joint, const bool subscribe) {
    if (!this->jointsSections.contains(joint->getNetwork())) return;

    auto&[section, joints] = this->jointsSections[joint->getNetwork()];
    assert(section != nullptr && "Tried to remove joint from renderer, but network does not exist");
    const auto it = std::ranges::find(joints, joint);
    if (it == joints.end()) return;
    const unsigned int networkJointIndex = std::distance(joints.begin(), it);
    assert(networkJointIndex < section->elements && "Tried to remove with invalid index");
    const bool deletedSection = this->jointBuffer.removeElement(section, networkJointIndex);
    joints.erase(it);
    assert((deletedSection && joints.empty()) ||
           (!deletedSection && !joints.empty()) && "Deleted section without deleting NetworkJoints");
    if (joints.empty()) {
        this->jointsSections.erase(joint->getNetwork());
        if (!this->wiresSections.contains(joint->getNetwork())) {
            joint->getNetwork()->unsubscribe(this);
        }
    }
    if (subscribe) {
        joint->Subject<MoveEvent>::unsubscribe(this);
        joint->Networkable::unsubscribe(this);
    }
    this->jointPositions.erase(joint);
    this->jointBuffer.bufferAll();
}

void CablingRenderer::addWire(Wire *wire, const bool subscribe) {
    const Color color = wire->getNetwork()->getColor();
    auto&[section, wires] = this->wiresSections[wire->getNetwork()];
    if (section == nullptr) {
        section = this->wireBuffer.createSection();
    } else if (std::ranges::find(wires, wire) != wires.end()) return;
    const VertexData startElement{wire->start->getPos(), color};
    const VertexData endElement{wire->end->getPos(), color};
    this->wireBuffer.addElement(startElement, section);
    this->wireBuffer.addElement(endElement, section);
    wires.push_back(wire);
    if (subscribe) {
        wire->Subject<NetworkChangeEvent>::subscribe(this);
    }
    wire->getNetwork()->subscribe(this);
    this->wireBuffer.bufferAll();
}

void CablingRenderer::removeWire(Wire *wire, const bool subscribe) {
    if (!this->wiresSections.contains(wire->getNetwork())) return;

    auto&[section, wires] = this->wiresSections[wire->getNetwork()];
    const auto it = std::ranges::find(wires, wire);
	if (it == wires.end()) return;
    const unsigned int networkWireIndex = std::distance(wires.begin(), it);
    this->wireBuffer.removeElement(section, networkWireIndex*2+1);
    this->wireBuffer.removeElement(section, networkWireIndex*2);
    wires.erase(it);
    if (wires.empty()) {
        this->wiresSections.erase(wire->getNetwork());
        if (!this->jointsSections.contains(wire->getNetwork())) {
            wire->getNetwork()->unsubscribe(this);
        }
    }
    if (subscribe) {
        wire->Subject<NetworkChangeEvent>::unsubscribe(this);
    }
    this->wireBuffer.bufferAll();
}

void CablingRenderer::notify(const MoveEvent& data) {
    if (!data.before) return;
    if (const auto joint = dynamic_cast<Joint*>(data.movable)) {
        this->moveJoint(joint, data.delta);
    } else if (const auto wire = dynamic_cast<Wire*>(data.movable)) {
        this->moveWire(wire, data.delta);
    }
}

void CablingRenderer::moveJoint(Joint *joint, glm::vec2 delta) {
    this->jointPositions[joint] = this->getJointPos(joint) + delta;
    this->updateJoint(joint, this->jointPositions[joint]);
    for (const auto &wire: joint->wires) {
        this->updateWire(wire, this->jointPositions[joint], wire->start == joint);
    }
}

void CablingRenderer::moveWire(Wire *wire, glm::vec2 delta) {
    this->moveJoint(wire->start, delta);
    this->moveJoint(wire->end, delta);
}



void CablingRenderer::notify(const NetworkChangeEvent &data) {
    if (data.before) {
        if (auto *joint = dynamic_cast<Joint *>(data.networkable)) {
            this->removeJoint(joint, false);
        } else if (const auto wire = dynamic_cast<Wire *>(data.networkable)) {
            this->removeWire(wire, false);
        }
    } else {
        if (auto *joint = dynamic_cast<Joint *>(data.networkable)) {
            this->addJoint(joint, false);
        } else if (const auto wire = dynamic_cast<Wire *>(data.networkable)) {
            this->addWire(wire, false);
        }
    }
}

void CablingRenderer::notify(const NetworkUpdateEvent &data) {
    this->updateNetwork(data.network);
}

glm::vec2 CablingRenderer::getJointPos(Joint *joint) {
    if (this->jointPositions.contains(joint)) {
        return this->jointPositions[joint];
    } else {
        this->jointPositions[joint] = joint->getPos();
        return joint->getPos();
    }
}
