//
// Created by felix on 25.03.25.
//

#ifndef MODELS_H
#define MODELS_H

#define CEREAL_ARCHIVE(...) template <class Archive> void serialize(Archive &archive) {archive(__VA_ARGS__);}

#include <cstdint>

#include "graph.hpp"

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
    
    namespace Serializer {
        struct SimulationNode {
            std::map<Sim::Node*, uint32_t> nodes;
            Models::SimulationNode simNode;

            template <class Archive>
            void save(Archive &archive) {
                if (const auto node = this->nodes.find(this->simNode.node); node != this->nodes.end()) {
                    archive(CEREAL_NVP(node->second));
                    return;
                }
                throw std::runtime_error("Failed to serialize SimulationNode Component with unknown Simulation Node");
            }
        };
    }
    namespace Deserializer {
        struct SimulationNode {
            std::map<uint32_t, Sim::Node*> nodes;
            Models::SimulationNode component;

            template <class Archive>
            void load(Archive &archive) {
                uint32_t id;
                archive(id);
                if (const auto node = this->nodes.find(id); node != this->nodes.end()) {
                    this->component.node = node->second;
                    return;
                }
                throw std::runtime_error("Failed to deserialize SimulationNode Component with unknown Simulation Node");
            }
        };
    }
}

#endif // MODELS_H
