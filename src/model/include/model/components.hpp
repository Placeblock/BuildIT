//
// Created by felix on 25.03.25.
//

#ifndef MODELS_H
#define MODELS_H

#include <cstdint>
#include <vector>

#include "../../../../lib/flecs/include/flecs/addons/cpp/flecs.hpp"
#include "simulation/node.hpp"

namespace Models {
    struct Position {
        static uint16_t version;
        int x;
        int y;
    };

    struct Rotation {
        static uint16_t version;
        uint8_t rot;
    };

    struct Joint {
        static uint16_t version;
        std::vector<uint32_t> joints;
    };

    struct AndGate {
        static uint16_t version;
    };

    struct NotGate {
        static uint16_t version;
    };

    struct OrGate {
        static uint16_t version;
    };

    struct Connection {
        static uint16_t version;
        bool output;
        int pinIndex;
    };
    
}

#endif // MODELS_H
