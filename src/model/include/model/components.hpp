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
        std::type_index type;
        uint16_t size;
        bool output;
        void *pin;
    };

    struct Joint;
    struct Wire {
        flecs::entity left;
        flecs::entity right;
        [[nodiscard]] flecs::entity getOther(const flecs::entity joint) const;
    };
    /**
     * ECS Tag for Joints
     */
    struct Joint {
        std::vector<Wire *> wires;
    };
}

#endif // MODELS_H
