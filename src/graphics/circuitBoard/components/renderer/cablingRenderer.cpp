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
    NetworkJoints& networkJoints = this->jointsSections[joint->getNetwork()];
    for (auto it = networkJoints.joints.begin(); it != networkJoints.joints.end() && *it != joint; ++it, ++networkJointIndex) {}
    this->jointBuffer.updateElement({newPos, joint->getNetwork()->getColor()},
                                    this->jointsSections[joint->getNetwork()].section, networkJointIndex);
}

void CablingRenderer::updateWire(Wire *wire, glm::vec2 pos, bool start) {
    if (!this->wiresSections.contains(wire->getNetwork())) {
        return;
    }
    this->wireBuffer.bind();
    Color color = wire->getNetwork()->getColor();
    unsigned int networkWireIndex=0;
    NetworkWires& networkWires = this->wiresSections[wire->getNetwork()];
    for (auto it = networkWires.wires.begin(); it != networkWires.wires.end() && *it != wire; ++it, ++networkWireIndex) {}
    unsigned int sectionIndex = networkWireIndex*2 + (start ? 0 : 1);
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

void CablingRenderer::addJoint(Joint *joint, bool subscribe) {
    NetworkJoints& networkJoints = this->jointsSections[joint->getNetwork()];
    if (networkJoints.section == nullptr) {
        networkJoints.section = this->jointBuffer.createSection();
    } else if (std::find(networkJoints.joints.begin(), networkJoints.joints.end(), joint) != networkJoints.joints.end()) return;
    VertexData element{joint->getPos(), joint->getNetwork()->getColor()};
    this->jointBuffer.addElement(element, networkJoints.section);
    networkJoints.joints.push_back(joint);
    if (subscribe) {
        joint->Movable::subscribe(this);
        joint->Networkable::subscribe(this);
    }
    this->jointBuffer.bufferAll();
    joint->getNetwork()->subscribe(this);
}

void CablingRenderer::removeJoint(Joint *joint, bool subscribe) {
    if (!this->jointsSections.contains(joint->getNetwork())) return;

    NetworkJoints& networkJoints = this->jointsSections[joint->getNetwork()];
    assert(networkJoints.section != nullptr && "Tried to remove joint from renderer, but network does not exist");
    auto it = std::find(networkJoints.joints.begin(), networkJoints.joints.end(), joint);
    assert(it != networkJoints.joints.end() && "Tried to remove joint from existing network that was never rendered");
    unsigned int networkJointIndex = std::distance(networkJoints.joints.begin(), it);
    assert(networkJointIndex < networkJoints.section->elements && "Tried to remove with invalid index");
    bool deletedSection = this->jointBuffer.removeElement(networkJoints.section, networkJointIndex);
    networkJoints.joints.erase(it);
    assert((deletedSection && networkJoints.joints.empty()) ||
           (!deletedSection && !networkJoints.joints.empty()) && "Deleted section without deleting NetworkJoints");
    if (networkJoints.joints.empty()) {
        this->jointsSections.erase(joint->getNetwork());
        if (!this->wiresSections.contains(joint->getNetwork())) {
            joint->getNetwork()->unsubscribe(this);
        }
    }
    if (subscribe) {
        joint->Subject<MoveEvent>::unsubscribe(this);
        joint->Networkable::unsubscribe(this);
    }
    this->jointBuffer.bufferAll();
}

void CablingRenderer::addWire(Wire *wire, bool subscribe) {
    Color color = wire->getNetwork()->getColor();
    NetworkWires& networkWires = this->wiresSections[wire->getNetwork()];
    if (networkWires.section == nullptr) {
        networkWires.section = this->wireBuffer.createSection();
    } else if (std::find(networkWires.wires.begin(), networkWires.wires.end(), wire) != networkWires.wires.end()) return;
    VertexData startElement{wire->start->getPos(), color};
    VertexData endElement{wire->end->getPos(), color};
    this->wireBuffer.addElement(startElement, networkWires.section);
    this->wireBuffer.addElement(endElement, networkWires.section);
    networkWires.wires.push_back(wire);
    if (subscribe) {
        wire->Subject<NetworkChangeEvent>::subscribe(this);
    }
    wire->getNetwork()->subscribe(this);
    this->wireBuffer.bufferAll();
}

void CablingRenderer::removeWire(Wire *wire, bool subscribe) {
    if (!this->wiresSections.contains(wire->getNetwork())) return;

    NetworkWires& networkWires = this->wiresSections[wire->getNetwork()];
    auto it = std::find(networkWires.wires.begin(), networkWires.wires.end(), wire);
	if (it == networkWires.wires.end()) return;
    unsigned int networkWireIndex = std::distance(networkWires.wires.begin(), it);
    this->wireBuffer.removeElement(networkWires.section, networkWireIndex*2+1);
    this->wireBuffer.removeElement(networkWires.section, networkWireIndex*2);
    networkWires.wires.erase(it);
    if (networkWires.wires.empty()) {
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
    if (Joint *joint = dynamic_cast<Joint*>(data.movable)) {	
        if (!data.before) return;
        this->updateJoint(joint, joint->getPos() + data.delta);
        for (const auto &wire: joint->wires) {
            this->updateWire(wire, joint->getPos() + data.delta, wire->start == joint);
        }
    }
}

void CablingRenderer::notify(const NetworkChangeEvent &data) {
    if (data.before) {
        if (Joint *joint = dynamic_cast<Joint *>(data.networkable)) {
            this->removeJoint(joint, false);
        } else if (Wire *wire = dynamic_cast<Wire *>(data.networkable)) {
            this->removeWire(wire, false);
        }
    } else {
        if (Joint *joint = dynamic_cast<Joint *>(data.networkable)) {
            this->addJoint(joint, false);
        } else if (Wire *wire = dynamic_cast<Wire *>(data.networkable)) {
            this->addWire(wire, false);
        }
    }
}

void CablingRenderer::notify(const NetworkUpdateEvent &data) {
    this->updateNetwork(data.network);
}
