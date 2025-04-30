//
// Created by felix on 30.04.25.
//

#ifndef WORLDREGISTRY_H
#define WORLDREGISTRY_H

#include <unordered_map>
#include <flecs.h>


class WorldRegistry {
    uint16_t nextWorldId = 0;
    std::unordered_map<uint16_t, flecs::world> worlds;

public:
    flecs::world &getWorld(uint16_t id);
    flecs::world &getDefaultWorld();
    uint16_t createWorld();
    void deleteWorld(uint16_t id);
};



#endif //WORLDREGISTRY_H
