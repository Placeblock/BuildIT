//
// Created by felix on 20.04.25.
//

#include "worlds/worldRegistry.hpp"

void createDefaultPipeline() {

}

int main() {
    WorldRegistry registry;

    registry.createWorld();
    flecs::world &world = registry.getDefaultWorld();

}
