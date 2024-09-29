//
// Created by felix on 9/29/24.
//

#include "serializer.h"
#include "graphics/circuitBoard/components/abstraction/component.h"

void Serializer::serialize(std::unordered_set<std::shared_ptr<Component>>* components) {
    for (const auto &component: *components) {
        uint16_t id = this->keyIDs.getID(component->getUniqueID());
        this->context.serialized.write((const char*)&id, 2);
        component->serialize(this->context);
    }
    constexpr uint16_t type = 3; // Type for simulation queue
    this->context.serialized.write((const char*)&type, 2); // Component type
    const uint32_t queueSize = this->updateQueue.size();
    this->context.serialized.write((const char*)&queueSize, sizeof(queueSize)); // Component type
    while (!this->updateQueue.empty()) {
        Sim::Node *node = this->updateQueue.front();
        const uint32_t id = this->context.nodeIDs.getID(node);
        this->context.serialized.write((const char*)&id, sizeof(id)); // Component type
        this->updateQueue.pop();
    }
}
