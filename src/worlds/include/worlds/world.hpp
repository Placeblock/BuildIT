//
// Created by felix on 11.05.25.
//

#ifndef WORLD_HPP
#define WORLD_HPP


class World {
    flecs::world &world;
    EventTarget<WorldEvent> eventTarget;

public:
    explicit World(flecs::world &world);
    ~World() = default;
};



#endif //WORLD_HPP
