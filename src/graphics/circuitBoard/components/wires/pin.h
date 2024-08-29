//
// Created by felix on 8/29/24.
//

#ifndef BUILDIT_PIN_H
#define BUILDIT_PIN_H

#include <cstdint>
#include "graphics/circuitBoard/components/nodes/node.h"

struct Pin {
    Node *node;
    uint8_t index;

public:
    [[nodiscard]] Sim::Node* getInputSimNode() const {
        return this->node->getInputSimNode(index).node;
    }
    [[nodiscard]] Sim::Node* getOutputSimNode() const {
        return this->node->getOutputSimNode(index).node;
    }

    bool operator==(const Pin &other) const {
        return (node == other.node && index == other.index);
    }
};

template <>
struct std::hash<Pin> {
    std::size_t operator()(const Pin& p) const {
        using std::size_t;
        using std::hash;
        using std::string;

        return ((hash<Node*>()(p.node) ^ (hash<int>()(p.index) << 1)) >> 1);
    }
};

#endif //BUILDIT_PIN_H
