//
// Created by felix on 8/29/24.
//

#ifndef BUILDIT_PIN_H
#define BUILDIT_PIN_H

#include <cstdint>
#include "simulation/node.h"

class Node;
class SimNodeData;

struct Pin {
    Node *node;
    uint8_t index;

    [[nodiscard]] Sim::Node* getInputSimNode() const;
    [[nodiscard]] Sim::Node* getOutputSimNode() const;
    [[nodiscard]] SimNodeData getInputSimData() const;
    [[nodiscard]] SimNodeData getOutputSimData() const;

    bool operator==(const Pin &other) const {
        return (node == other.node && index == other.index);
    }
};

template <>
struct std::hash<Pin> {
    std::size_t operator()(const Pin& p) const noexcept {
        using std::size_t;
        using std::hash;
        using std::string;

        return ((hash<Node*>()(p.node) ^ (hash<int>()(p.index) << 1)) >> 1);
    }
};

#endif //BUILDIT_PIN_H
