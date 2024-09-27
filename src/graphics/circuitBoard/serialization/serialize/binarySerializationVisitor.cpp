//
// Created by felix on 9/27/24.
//

#include "binarySerializationVisitor.h"

#include <utility>
#include "graphics/circuitBoard/components/nodes/notGate.h"
#include "graphics/circuitBoard/components/cabling/wire.h"

void BinarySerializationVisitor::serialize() {
    for (const auto &component: this->components) {
        component->visit(this);
    }
}

void BinarySerializationVisitor::doFor(NotGate *notGate) {
    this->serialized << 0x0000; // Component type
    intVec2 cell = intVec2(notGate->getPos() / 32.0f);
    this->serialized << cell.x << cell.y;
    this->serialized << notGate->getRotation();
    this->serialized << notGate->getInput();
    this->serialized << this->networkIDs.getID(notGate->outputNetworks[0]);
}

void BinarySerializationVisitor::doFor(Joint *joint) {
    this->serialized << 0x0001; // Component type
    this->serialized << this->jointIDs.getID(joint);
    intVec2 cell = intVec2(joint->getPos() / 32.0f);
    this->serialized << cell.x << cell.y;
    this->serialized << this->networkIDs.getID(joint->getNetwork().get());
}

void BinarySerializationVisitor::doFor(Wire *wire) {
    this->serialized << 0x0002; // Component type
    this->serialized << this->jointIDs.getID(wire->start);
    this->serialized << this->jointIDs.getID(wire->end);
}

BinarySerializationVisitor::BinarySerializationVisitor(std::unordered_set<Component *> components)
        : SerializationVisitor(std::move(components)) {

}
