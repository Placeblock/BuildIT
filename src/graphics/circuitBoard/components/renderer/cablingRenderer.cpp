//
// Created by felix on 8/2/24.
//

#include "cablingRenderer.h"
#include "graphics/util.h"

CablingRenderer::CablingRenderer() :
                                 jointVertexBuffer(GL_ARRAY_BUFFER, Util::getDefaultVertexLayout()),
                                 jointColorBuffer(GL_ARRAY_BUFFER, Util::getDefaultColorLayout()),
                                 wireVertexBuffer(GL_ARRAY_BUFFER, Util::getDefaultVertexLayout()),
                                 wireColorBuffer(GL_ARRAY_BUFFER, Util::getDefaultColorLayout()) {
    this->jointVA.addBuffer(&this->jointVertexBuffer);
    this->jointVA.addBuffer(&this->jointColorBuffer);
    this->wireVA.addBuffer(&this->wireVertexBuffer);
    this->wireVA.addBuffer(&this->wireColorBuffer);
}

void CablingRenderer::drawWires(const Program *shader) {
    //this->wireColorBuffer.bufferAll();
    if (this->wireVertexBuffer.size() != 0) {
        shader->use();
        this->wireVA.bind();
        glDrawArrays(GL_LINES, 0, this->wireVertexBuffer.size());
    }
}

void CablingRenderer::drawJoints(const Program *shader) {
    //this->jointColorBuffer.bufferAll();
    if (this->jointVertexBuffer.size() != 0) {
        shader->use();
        this->jointVA.bind();
        glDrawArrays(GL_POINTS, 0, this->jointVertexBuffer.size());
    }
}

void CablingRenderer::render(const Program *wireShader, const Program *jointShader) {
    this->drawWires(wireShader);
    this->drawJoints(jointShader);
}

ElementData CablingRenderer::getJointData(const Joint *joint) {
    unsigned int networkJointIndex=0;
    if (!this->jointsSections.contains(joint->getNetwork())) return {nullptr, nullptr, -1};
    auto&[vertexSection, colorSection, joints] = this->jointsSections[joint->getNetwork()];
    auto it = joints.begin();
    for (; it != joints.end() && *it != joint; ++it, ++networkJointIndex) {}
    if (it == joints.end()) return {vertexSection, colorSection, -1};
    return {vertexSection, colorSection, int(networkJointIndex)};
}

void CablingRenderer::updateJoint(const Joint *joint, const glm::vec2 newPos) {
    auto [vertexSection, colorSection, index] = this->getJointData(joint);
    if (vertexSection == nullptr || index == -1) return;
    this->jointVertexBuffer.bind();
    this->jointVertexBuffer.updateElement(newPos, vertexSection, index);
}

void CablingRenderer::updateJoint(const Joint *joint, const Color newColor) {
    auto [vertexSection, colorSection, index] = this->getJointData(joint);
    if (colorSection == nullptr || index == -1) return;
    this->jointColorBuffer.bind();
    this->jointColorBuffer.updateElement(newColor, colorSection, index);
}

ElementData CablingRenderer::getWireData(const Wire *wire) {
    if (!this->wiresSections.contains(wire->getNetwork())) return {nullptr, nullptr, -1};
    unsigned int networkWireIndex=0;
    auto&[vertexSection, colorSection, wires] = this->wiresSections[wire->getNetwork()];
    auto it = wires.begin();
    for (; it != wires.end() && *it != wire; ++it, ++networkWireIndex) {}
    if (it == wires.end()) return {vertexSection, colorSection, -1};
    return  {vertexSection, colorSection, int(networkWireIndex)};
}

void CablingRenderer::updateWire(const Wire *wire, const glm::vec2 newPos, const bool start) {
    auto [vertexSection, colorSection, index] = this->getWireData(wire);
    if (vertexSection == nullptr || index == -1) return;
    const unsigned int sectionIndex = index*2 + (start ? 0 : 1);
    this->wireVertexBuffer.bind();
    this->wireVertexBuffer.updateElement(newPos, vertexSection, sectionIndex);
}

void CablingRenderer::updateWire(const Wire *wire, Color newColor) {
    auto [vertexSection, colorSection, index] = this->getWireData(wire);
    if (colorSection == nullptr || index == -1) return;
    this->wireColorBuffer.bind();
    this->wireColorBuffer.updateElement(newColor, colorSection, index*2);
    this->wireColorBuffer.updateElement(newColor, colorSection, index*2+1);
}

void CablingRenderer::updateNetwork(Network *network) {
    if (this->wiresSections.contains(network)) {
        this->wireColorBuffer.bind();
        std::vector<Color> wireData;
        for (const auto &wire: this->wiresSections[network].wires) {
            wireData.push_back(wire->getColor());
            wireData.push_back(wire->getColor());
        }
        this->wireColorBuffer.updateSection(this->wiresSections[network].colorSection, wireData);
    }
    if (this->jointsSections.contains(network)) {
        this->jointColorBuffer.bind();
        std::vector<Color> jointData;
        for (const auto &joint: this->jointsSections[network].joints) {
            jointData.push_back(joint->getColor());
        }
        this->jointColorBuffer.updateSection(this->jointsSections[network].colorSection, jointData);
    }
}

void CablingRenderer::addJoint(Joint *joint, const bool subscribe) {
    auto&[vertexSection, colorSection, joints] = this->jointsSections[joint->getNetwork()];
    if (std::ranges::find(joints, joint) != joints.end()) return;
    if (vertexSection == nullptr) {
        vertexSection = this->jointVertexBuffer.createSection();
        colorSection = this->jointColorBuffer.createSection();
    }
    this->jointVertexBuffer.addElement(joint->getPos(), vertexSection);
    this->jointColorBuffer.addElement(joint->getNetwork()->getColor(), colorSection);
    joints.push_back(joint);
    if (subscribe) {
        joint->Movable::subscribe(this);
        joint->Networkable::subscribe(this);
        joint->Subject<SelectEvent>::subscribe(this);
        joint->Subject<DeselectEvent>::subscribe(this);
    }
    this->jointVertexBuffer.bufferAll();
    this->jointColorBuffer.bufferAll();
    joint->getNetwork()->subscribe(this);
    this->jointPositions[joint] = joint->getPos();
}

void CablingRenderer::removeJoint(Joint *joint, const bool subscribe) {
    this->jointPositions.erase(joint);
    if (!this->jointsSections.contains(joint->getNetwork())) return;
    auto&[vertexSection, colorSection, joints] = this->jointsSections[joint->getNetwork()];
    assert((vertexSection != nullptr) && "Tried to remove joint from renderer, but network does not exist");
    const auto it = std::ranges::find(joints, joint);
    if (it == joints.end()) return;
    const unsigned int networkJointIndex = std::distance(joints.begin(), it);
    assert((networkJointIndex < vertexSection->elements) && "Tried to remove with invalid index");
    const bool deletedSection = this->jointVertexBuffer.removeElement(vertexSection, networkJointIndex);
    this->jointColorBuffer.removeElement(colorSection, networkJointIndex);
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
        joint->Subject<SelectEvent>::unsubscribe(this);
        joint->Subject<DeselectEvent>::unsubscribe(this);
    }
    this->jointVertexBuffer.bufferAll();
    this->jointColorBuffer.bufferAll();
}

void CablingRenderer::addWire(Wire *wire, const bool subscribe) {
    const Color color = wire->getNetwork()->getColor();
    auto&[vertexSection, colorSection, wires] = this->wiresSections[wire->getNetwork()];
    if (vertexSection == nullptr) {
        vertexSection = this->wireVertexBuffer.createSection();
        colorSection = this->wireColorBuffer.createSection();
    } else if (std::ranges::find(wires, wire) != wires.end()) return;
    this->wireVertexBuffer.addElement(wire->start->getPos(), vertexSection);
    this->wireVertexBuffer.addElement(wire->end->getPos(), vertexSection);
    this->wireColorBuffer.addElement(color, colorSection);
    this->wireColorBuffer.addElement(color, colorSection);
    wires.push_back(wire);
    if (subscribe) {
        wire->Subject<NetworkChangeEvent>::subscribe(this);
        wire->Subject<MoveEvent>::subscribe(this);
        wire->Subject<SelectEvent>::subscribe(this);
        wire->Subject<DeselectEvent>::subscribe(this);
    }
    wire->getNetwork()->subscribe(this);
    this->wireVertexBuffer.bufferAll();
    this->wireColorBuffer.bufferAll();
}

void CablingRenderer::removeWire(Wire *wire, const bool subscribe) {
    if (!this->wiresSections.contains(wire->getNetwork())) return;

    auto&[vertexSection, colorSection, wires] = this->wiresSections[wire->getNetwork()];
    const auto it = std::ranges::find(wires, wire);
	if (it == wires.end()) return;
    const unsigned int networkWireIndex = std::distance(wires.begin(), it);
    this->wireVertexBuffer.removeElement(vertexSection, networkWireIndex*2+1);
    this->wireVertexBuffer.removeElement(vertexSection, networkWireIndex*2);
    this->wireColorBuffer.removeElement(colorSection, networkWireIndex*2+1);
    this->wireColorBuffer.removeElement(colorSection, networkWireIndex*2);
    wires.erase(it);
    if (wires.empty()) {
        this->wiresSections.erase(wire->getNetwork());
        if (!this->jointsSections.contains(wire->getNetwork())) {
            wire->getNetwork()->unsubscribe(this);
        }
    }
    if (subscribe) {
        wire->Subject<NetworkChangeEvent>::unsubscribe(this);
        wire->Subject<MoveEvent>::unsubscribe(this);
        wire->Subject<SelectEvent>::unsubscribe(this);
        wire->Subject<DeselectEvent>::unsubscribe(this);
    }
    this->wireVertexBuffer.bufferAll();
    this->wireColorBuffer.bufferAll();
}

void CablingRenderer::notify(const MoveEvent& data) {
    if (!data.before) return;
    if (const auto joint = dynamic_cast<Joint*>(data.movable)) {
        this->moveJoint(joint, data.delta);
    }
}

void CablingRenderer::moveJoint(Joint *joint, glm::vec2 delta) {
    this->jointPositions[joint] = this->getJointPos(joint) + delta;
    this->updateJoint(joint, this->jointPositions[joint]);
    for (const auto &wire: joint->wires) {
        this->updateWire(wire, this->jointPositions[joint], wire->start == joint);
    }
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

void CablingRenderer::notify(const SelectEvent &data) {
    if (const auto joint = dynamic_cast<Joint*>(data.selectable)) {
        this->updateJoint(joint, joint->getColor());
    } else if (const auto wire = dynamic_cast<Wire*>(data.selectable)) {
        this->updateWire(wire, wire->getColor());
    }
}

void CablingRenderer::notify(const DeselectEvent &data) {
    if (const auto joint = dynamic_cast<Joint*>(data.selectable)) {
        this->updateJoint(joint, joint->getColor());
    } else if (const auto wire = dynamic_cast<Wire*>(data.selectable)) {
        this->updateWire(wire, wire->getColor());
    }
}

glm::vec2& CablingRenderer::getJointPos(Joint *joint) {
    if (!this->jointPositions.contains(joint)) {
        this->jointPositions[joint] = joint->getPos();
    }
    return this->jointPositions[joint];
}