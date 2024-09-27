//
// Created by felix on 9/27/24.
//

#include "binarySerializationVisitor.h"

#include <utility>
#include "graphics/circuitBoard/components/nodes/notGate.h"
#include "graphics/circuitBoard/components/cabling/wire.h"

void BinarySerializationVisitor::serialize() {
    for (const auto &component: *this->components) {
        component->visit(this);
    }
    constexpr uint16_t type = 3; // Type for simulation queue
    this->serialized.write((const char*)&type, 2); // Component type
    const uint32_t queueSize = this->updateQueue.size();
    this->serialized.write((const char*)&queueSize, sizeof(queueSize)); // Component type
    while (!this->updateQueue.empty()) {
        Sim::Node *node = this->updateQueue.front();
        const uint32_t id = this->nodeIDs.getID(node);
        this->serialized.write((const char*)&id, sizeof(id)); // Component type
        this->updateQueue.pop();
    }
}

void BinarySerializationVisitor::doFor(NotGate *notGate) {
    constexpr uint16_t type = 0;
    this->serialized.write((const char*)&type, 2); // Component type
    uint32_t id = this->nodeIDs.getID(notGate->simNode.get());
    this->serialized.write((const char*)&id, sizeof(id));
    intVec2 cell = intVec2(notGate->getPos() / 32.0f);
    this->serialized.write((const char*)&cell.x, sizeof(cell.x));
    this->serialized.write((const char*)&cell.y, sizeof(cell.y));
    float rotation = notGate->getRotation();
    this->serialized.write((const char*)&rotation, sizeof(rotation));
    uint32_t input = notGate->getInput();
    this->serialized.write((const char*)&input, sizeof(input));
    uint32_t networkID = this->networkIDs.getID(notGate->outputNetworks[0]);
    this->serialized.write((const char*)&networkID, sizeof(networkID));
}

void BinarySerializationVisitor::doFor(Joint *joint) {
    constexpr uint type = 1;
    this->serialized.write((const char*)&type, 2); // Component type
    const uint32_t id = this->jointIDs.getID(joint);
    this->serialized.write((const char*)&id, sizeof(id));
    intVec2 cell = intVec2(joint->getPos() / 32.0f);
    this->serialized.write((const char*)&cell.x, sizeof(cell.x));
    this->serialized.write((const char*)&cell.y, sizeof(cell.y));
    uint32_t networkID = this->networkIDs.getID(joint->getNetwork().get());
    this->serialized.write((const char*)&networkID, sizeof(networkID));
}

void BinarySerializationVisitor::doFor(Wire *wire) {
    constexpr uint type = 2;
    this->serialized.write((const char*)&type, 2); // Component type
    const uint32_t startID = this->jointIDs.getID(wire->start);
    const uint32_t endID = this->jointIDs.getID(wire->end);
    this->serialized.write((const char*)&startID, sizeof(startID));
    this->serialized.write((const char*)&endID, sizeof(endID));
}

BinarySerializationVisitor::BinarySerializationVisitor(std::unordered_set<std::shared_ptr<Component>>* components, std::queue<Sim::Node*> updateQueue)
        : SerializationVisitor(components, updateQueue) {

}
