//
// Created by felix on 8/2/24.
//

#include "cablingRenderer.h"
#include "graphics/util.h"
#include "graphics/circuitBoard/components/cabling/wire.h"

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
    this->wireColorBuffer.bufferAll();
    if (this->wireVertexBuffer.size() != 0) {
        shader->use();
        this->wireVA.bind();
        glDrawArrays(GL_LINES, 0, this->wireVertexBuffer.size());
    }
}

void CablingRenderer::drawJoints(const Program *shader) {
    this->jointColorBuffer.bufferAll();
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

void CablingRenderer::updateJoint(Joint *joint, const glm::vec2 newPos) {
    if (!this->jointsIndices.contains(joint)) return;
    JointIndices jointIndices = this->jointsIndices[joint];
    this->jointVertexBuffer.bind();
    this->jointVertexBuffer.updateElement(jointIndices.vertexIndex, newPos, true);
}

void CablingRenderer::updateWire(Wire *wire, const glm::vec2 newPos, const bool start) {
    if (!this->wiresIndices.contains(wire)) return;
    WireIndices wireIndices = this->wiresIndices[wire];
    this->wireVertexBuffer.bind();
    this->wireVertexBuffer.updateElement(start ? wireIndices.startVertexIndex : wireIndices.endVertexIndex, newPos, true);
}

void CablingRenderer::updateJoint(Joint *joint, const Color newColor) {
    if (!this->jointsIndices.contains(joint)) return;
    JointIndices jointIndices = this->jointsIndices[joint];
    this->jointColorBuffer.updateElement(jointIndices.colorIndex, newColor, false);
}

void CablingRenderer::updateWire(Wire *wire, Color newColor) {
    if (!this->wiresIndices.contains(wire)) return;
    WireIndices wireIndices = this->wiresIndices[wire];
    this->wireColorBuffer.updateElement(wireIndices.startColorIndex, newColor, false);
}

void CablingRenderer::updateNetwork(Network *network) {
    for (const auto &wire: network->wires) {
        this->updateWire(wire, wire->getColor());
    }
    for (const auto &joint: network->joints) {
        this->updateJoint(joint, joint->getColor());
    }
}

void CablingRenderer::addJoint(Joint *joint, const bool subscribe) {
    if (this->jointsIndices.contains(joint)) return;
    Index* vertexIndex = this->jointVertexBuffer.addElement(joint->getPos());
    Index* colorIndex = this->jointColorBuffer.addElement(joint->getNetwork()->getRenderedColor());
    this->jointsIndices[joint] = JointIndices{vertexIndex, colorIndex};
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
    if (!this->jointsIndices.contains(joint)) return;
    JointIndices indices = this->jointsIndices[joint];
    this->jointColorBuffer.removeElement(indices.colorIndex);
    this->jointVertexBuffer.removeElement(indices.vertexIndex);
    this->jointsIndices.erase(joint);
    this->checkNetworkUnsubscribe(joint->getNetwork().get());
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
    if (this->wiresIndices.contains(wire)) return;
    const Color color = wire->getNetwork()->getRenderedColor();
    Index * startVertexIndex = this->wireVertexBuffer.addElement(wire->start->getPos());
    Index * endVertexIndex = this->wireVertexBuffer.addElement(wire->end->getPos());
    Index * startColorIndex = this->wireColorBuffer.addElement(color);
    Index * endColorIndex = this->wireColorBuffer.addElement(color);
    this->wiresIndices[wire] = WireIndices{startVertexIndex, startColorIndex, endVertexIndex, endColorIndex};
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
    if (!this->wiresIndices.contains(wire)) return;
    WireIndices indices = this->wiresIndices[wire];
    this->wireVertexBuffer.removeElement(indices.startVertexIndex);
    this->wireVertexBuffer.removeElement(indices.endVertexIndex);
    this->wireColorBuffer.removeElement(indices.startColorIndex);
    this->wireColorBuffer.removeElement(indices.endColorIndex);
    this->wiresIndices.erase(wire);
    this->checkNetworkUnsubscribe(wire->getNetwork().get());
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
    this->jointPositions[joint] = (this->jointPositions.contains(joint) ? this->jointPositions[joint] : joint->getPos()) + delta;
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
    /*this->updatePool.queue([this, data](){
        this->updateNetwork(data.network);
    });*/
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

bool CablingRenderer::hasNetwork(Network *network) {
    for (const auto &item: network->wires) {
        if (this->wiresIndices.contains(item)) return true;
    }
    return std::any_of(network->joints.begin(), network->joints.end(), [this](const auto& joint){
        return this->jointsIndices.contains(joint);
    });
}

void CablingRenderer::checkNetworkUnsubscribe(Network *network) {
    if (!this->hasNetwork(network)) {
        network->unsubscribe(this);
    }
}