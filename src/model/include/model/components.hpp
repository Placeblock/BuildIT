//
// Created by felix on 25.03.25.
//

#ifndef MODELS_H
#define MODELS_H

#include <cstdint>
#include <vector>

#include <simulation/node.hpp>

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

    struct SimulationNode {
        static uint16_t version;
        Sim::Node *node;
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
    
}

#endif // MODELS_H
