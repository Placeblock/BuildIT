//
// Created by felix on 13.07.24.
//

#ifndef BUILDIT_NODE_H
#define BUILDIT_NODE_H

#include <cstdint>
#include <memory>
#include <vector>
#include <queue>

namespace Simulation {
    class Node;

    class Pin {
    public:
        Node* targetNode;
        uint8_t targetIndex;
    };

    class Node {
    public:
        Node(uint8_t inputs, uint8_t outputs);
        virtual void update();
        uint32_t input = 0;
        uint32_t output = 0;
        uint32_t inputMask = 0;
        uint32_t outputMask = 0;
        std::vector<Pin> parents;
        std::vector<std::vector<Pin>> children;
        void setInput(uint8_t index, bool value);
        void setOutput(uint8_t index, bool value);
        bool getInput(uint8_t index) const;
        bool getOutput(uint8_t index) const;
        void recalculateInputMask();
        void recalculateOutputMask();
    };

    void connect(Pin parent, Pin child);

    void disconnect(Pin parent, Pin child);

    void update(std::queue<Node*>* queue, Node* node);
}

#endif //BUILDIT_NODE_H
