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
        bool disconnect;
    };

    /**
     * ECS Component for BuildIT Components that can be rotated
     */
    struct Rotation {
        uint8_t rot;

        Rotation &operator+=(const Rotation& rhs);
        Rotation operator+(const Rotation & rhs) const;

        void apply(Position& pos) const;
    };

    struct Rotate {
        Rotation delta;
        bool disconnect;
    };

    /**
     * ECS Component for BuildIT Components that have Simulation Nodes attached to it
     */
    struct Nodes {
        std::vector<std::unique_ptr<Sim::Node>> nodes;
    };
    
    /**
     * ECS Component for Input Pins
     */
    struct PinSink {
        Position position;
        std::type_index type;
        std::vector<void *> simPins;

        Position getAbs(Position componentPos, const Rotation *rot) const;
    };
    /**
     * ECS Component for Output Pins
     */
    struct Pin {
        Position position;
        std::type_index type;
        std::vector<void *> simPins;
        flecs::entity joint = flecs::entity::null();

        Position getAbs(Position componentPos, const Rotation *rot) const;
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
     * ECS Relationship for Connected Entities
     * Used by Pins and Joints to Connect each other
     * Could it be used by Pins and Pins too?
     */
    struct IsConnected {};
}

#endif // MODELS_H
