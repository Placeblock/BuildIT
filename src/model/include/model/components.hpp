//
// Created by felix on 25.03.25.
//

#ifndef MODELS_H
#define MODELS_H

#include <cstdint>
#include <string>
#include <vector>

#include "simulation/node.hpp"

namespace Models {
    template<typename T, unsigned int N>
    struct PinSink;
    
    template<typename T, unsigned int N>
    struct Pin {
        std::string name;
        std::array<std::vector<PinSink<T, N>*>, N> children;
        std::array<Sim::Pin<T>, N> simPins;
    };

    template<typename T, unsigned int N>
    struct PinSink {
        std::string name;
        Pin<T, N>* parent = nullptr;
        std::array<Sim::Pin<T>, N> simPins;
    };
    
    struct Position {
        int x;
        int y;
    };

    struct Rotation {
        uint8_t rot;
    };
}

#endif // MODELS_H
