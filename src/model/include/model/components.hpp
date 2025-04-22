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
        Sim::AndNode node;
        std::vector<PinSink<bool, 1>> inputPins;
        Pin<bool, 1> outputPin;
    };

    struct NotGate {
        static uint16_t version;
        Sim::NotNode node;
        PinSink<bool, 1> inputPin;
        Pin<bool, 1> outputPin;
    };

    struct OrGate {
        static uint16_t version;
        Sim::OrNode node;
        std::vector<PinSink<bool, 1>> inputPins;
        Pin<bool, 1> outputPin;
    };
    
}

#endif // MODELS_H
