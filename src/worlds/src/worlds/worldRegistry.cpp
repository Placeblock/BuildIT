//
// Created by felix on 30.04.25.
//

#include "worlds/worldRegistry.h"

flecs::world & WorldRegistry::getWorld(const uint16_t id) {
    return this->worlds[id];
}

flecs::world & WorldRegistry::getDefaultWorld() {
    return this->worlds[0];
}

uint16_t WorldRegistry::createWorld() {
    this->worlds[this->nextWorldId] = flecs::world();
    return this->nextWorldId++;
}

void WorldRegistry::deleteWorld(const uint16_t id) {
    this->worlds.erase(id);
}
