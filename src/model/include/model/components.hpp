//
// Created by felix on 25.03.25.
//

#ifndef MODELS_H
#define MODELS_H

#include <memory>
#include <typeindex>
#include <vector>
#include <flecs.h>

#include "simulation/node.hpp"

namespace Models {
    /**
     * ECS Component for BuildIT Components that can be positioned (all?)
     */
    struct Position {
        int x, y;

        explicit Position(const int x = 0, const int y = 0): x(x), y(y) {}

        Position &operator+=(const Position& rhs);
        Position operator+(const Position & rhs) const;

        Position &operator*=(int i);

        bool operator==(const Position & pos) const;
    };

    struct Move {
        Position delta;
    };

    /**
     * ECS Component for BuildIT Components that can be rotated
     */
    struct Rotation {
        uint8_t rot;

        void apply(Position& pos) const;
    };

    struct Rotate {
        Rotation delta;
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

        Position getAbs(const Position componentPos, const Rotation *rot) const;
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
        std::vector<flecs::entity> wires;
        std::vector<flecs::entity> joints;
    };
    /**
     * ECS Tag for Joints
     */
    struct Joint {};
}

#endif // MODELS_H
