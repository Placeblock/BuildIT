//
// Created by felix on 25.03.25.
//

#ifndef MODELS_H
#define MODELS_H

#include <cstdint>
#include <memory>
#include <typeindex>
#include <vector>

#include "flecs/addons/cpp/flecs.hpp"
#include "simulation/node.hpp"

namespace Models {
    /**
     * ECS Component for BuildIT Components that can be positioned (all?)
     */
    struct Position {
        int x;
        int y;
    };

    /**
     * ECS Component for BuildIT Components that can be rotated
     */
    struct Rotation {
        uint8_t rot;
    };

    /**
     * ECS Component for BuildIT Components that have Simulation Nodes attached to it
     */
    struct Nodes {
        std::vector<std::unique_ptr<Sim::Node>> nodes;
    };
    
    /**
     * ECS Component for BuildIT Components with Pins
     */
    struct Pin {
        Position position;
        std::type_index type;
        uint16_t size;
        bool output;
        void *pin;
    };

    /**
     * ECS Component for a Wire
     */
    struct Wire {
        flecs::entity left;
        flecs::entity right;
        [[nodiscard]] flecs::entity getOther(flecs::entity joint) const;
    };
    struct HasWire {};
    /**
     * ECS Component for a Network
     */
    struct Network {
        std::vector<std::unique_ptr<Wire>> wires;
        std::vector<flecs::entity> joints;
    };
    /**
     * ECS Tag for Joints
     */
    struct Joint {};

    flecs::observer pinObserver(const flecs::world &world) {
        return world.observer<Pin>()
            .event(flecs::OnSet)
            .each([](flecs::entity e, Pin& pin) {
                
            });
    }

}

#endif // MODELS_H
