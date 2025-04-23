//
// Created by felix on 12.04.25.
//

#ifndef CONTROLLER_HPP
#define CONTROLLER_HPP
#include "model/network.hpp"
#include "flecs.h"

inline flecs::world world;
inline flecs::system sys = world.system<Models::Joint>().each([](flecs::entity e, Models::Joint& joint) {
    for (auto it = joint.wires.begin(); it != joint.wires.end(); ) {
        if (flecs::entity other = wire->getOther(e); world.is_alive(other)) {
            ++it;
        } else {
            e.destruct();
        }
    }
});

#endif //CONTROLLER_HPP
