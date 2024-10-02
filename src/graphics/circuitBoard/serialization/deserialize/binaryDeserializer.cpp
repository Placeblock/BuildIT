//
// Created by felix on 9/27/24.
//

#include <cassert>
#include "binaryDeserializer.h"
#include "graphics/circuitBoard/components/cabling/wire.h"
#include "graphics/circuitBoard/components/cabling/joint.h"
#include "graphics/circuitBoard/components/nodes/notGate.h"
#include "simulation/updaters.h"

void BinaryDeserializer::deserialize(std::istream &in) {
    while(!in.eof()) {
        uint16_t id;
        in.read((char*)&id, 2);
        if (in.eof()) return;
        if (id == 0x0000) {
            this->deserializeNotGate(in);
        } else if (id == 0x0001) {
            this->deserializeJoint(in);
        } else if (id == 0x0002) {
            this->deserializeWire(in);
        } else if (id == 0x0003) {
            uint32_t queueSize;
            in.read((char*)&queueSize, 4);
            for (size_t i = 0; i < queueSize; ++i) {
                uint32_t nodeID;
                in.read((char*)&nodeID, 4);
                Sim::Node *node = this->nodes[nodeID];
                this->updateQueue.push(node);
            }
        } else {
            assert(("Unknown component ID " + std::to_string(id)).c_str());
        }
    }
}

void BinaryDeserializer::deserializeNotGate(std::istream &in) {
    int32_t x;
    int32_t y;
    uint32_t id;
    float rotation;
    uint32_t input;
    uint32_t networkID;
    in.read((char*)&id, 4);
    in.read((char*)&x, 4);
    in.read((char*)&y, 4);
    in.read((char*)&rotation, 4);
    in.read((char*)&input, 4);
    in.read((char*)&networkID, 4);
    std::shared_ptr<Network> network = this->getNetwork(networkID);
    auto simGate = std::make_shared<Sim::Node>(1, 1, std::make_unique<Sim::NotUpdater>(), input);
    std::shared_ptr<NotGate> gate = std::make_shared<NotGate>(glm::vec2{x*32, y*32}, simGate);
    this->nodes[id] = simGate.get();
    gate->outputNetworks[0] = network.get();
    gate->rotate(rotation);
    this->components.insert(gate);
}

void BinaryDeserializer::deserializeJoint(std::istream &in) {
    uint32_t id;
    int32_t x;
    int32_t y;
    uint32_t networkID;
    in.read((char*)&id, 4);
    in.read((char*)&x, 4);
    in.read((char*)&y, 4);
    in.read((char*)&networkID, 4);
    std::shared_ptr<Network> network = this->getNetwork(networkID);
    std::shared_ptr<Joint> joint = std::make_shared<Joint>(glm::vec2{x*32, y*32}, network);
    network->joints.push_back(joint.get());
    this->joints[id] = joint.get();
    this->components.insert(joint);
    if (this->waitingWiresStart.contains(id)) {
        for (const auto &wire: this->waitingWiresStart[id]) {
            wire->start = joint.get();
            wire->setNetwork(joint->getNetwork());
            network->wires.push_back(wire);
            this->checkWire(wire);
        }
    }
    if (this->waitingWiresEnd.contains(id)) {
        for (const auto &wire: this->waitingWiresEnd[id]) {
            wire->end = joint.get();
            this->checkWire(wire);
        }
    }
}

void BinaryDeserializer::deserializeWire(std::istream &in) {
    uint32_t startJointID;
    uint32_t endJointID;
    in.read((char*)&startJointID, 4);
    in.read((char*)&endJointID, 4);
    std::shared_ptr<Wire> wire = std::make_shared<Wire>(nullptr, nullptr);
    if (this->joints.contains(startJointID)) {
        wire->start = this->joints[startJointID];
        wire->setNetwork(wire->start->getNetwork());
        wire->getNetwork()->wires.push_back(wire.get());
    } else {
        this->waitingWiresStart[startJointID].insert(wire.get());
    }
    if (this->joints.contains(endJointID)) {
        wire->end = this->joints[endJointID];
        this->checkWire(wire.get());
    } else {
        this->waitingWiresEnd[endJointID].insert(wire.get());
    }
    this->components.insert(wire);
}

std::shared_ptr<Network> BinaryDeserializer::getNetwork(uint32_t id) {
    if (this->networks.contains(id)) return this->networks[id];
    this->networks[id] = std::make_shared<Network>();
    return this->networks[id];
}

void BinaryDeserializer::checkWire(Wire *wire) {
    if (wire->start != nullptr && wire->end != nullptr) {
        wire->connect();
    }
}
