//
// Created by felix on 9/29/24.
//

#include "serializer.h"
#include "graphics/circuitBoard/components/abstraction/component.h"

void Serializer::serialize(std::unordered_set<std::shared_ptr<Component>>* components,
                           std::queue<Sim::Node*> updateQueue) {
    for (const auto &component: *components) {
        uint16_t id = this->keyIDs.getID(component->getUniqueID());
        this->context.serialized.write((const char*)&id, 2);
        component->serialize(this->context);
    }
    constexpr uint16_t type = 3; // Type for simulation queue
    this->context.serialized.write((const char*)&type, 2); // Component type
    const uint32_t queueSize = updateQueue.size();
    this->context.serialized.write((const char*)&queueSize, sizeof(queueSize)); // Component type
    while (!updateQueue.empty()) {
        Sim::Node *node = updateQueue.front();
        const uint32_t id = this->context.nodeIDs.getID(node);
        this->context.serialized.write((const char*)&id, sizeof(id)); // Component type
        updateQueue.pop();
    }
    uint16_t componentTypes = this->keyIDs.ids.size();
    this->context.serialized.write((const char*)&componentTypes, 2);
    for (const auto &[id, serializedID]: this->keyIDs.ids) {
        uint8_t idSize = id.size();
        this->context.serialized.write((const char*)&idSize, 1);
        this->context.serialized.write((const char*)&id, idSize);
        this->context.serialized.write((const char*)&serializedID, 2);
    }

}
