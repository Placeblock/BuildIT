//
// Created by felix on 13.07.24.
//

#ifndef BUILDIT_S_NODE_H
#define BUILDIT_S_NODE_H

#include <cstdint>
#include <memory>
#include <vector>
#include <queue>

namespace Sim {
    class Node;

    class Pin {
    public:
        Node* node;
        Node* targetNode;
        uint8_t targetIndex;
    };

    class Node {
    public:
        Node(uint8_t inputs, uint8_t outputs);
        virtual void update();
        uint32_t input = 0;
        uint32_t output = 0;
        void setInput(uint8_t index, bool value);
        void setOutput(uint8_t index, bool value);
        [[nodiscard]] bool getInput(uint8_t index) const;
        [[nodiscard]] bool getOutput(uint8_t index) const;
        void recalculateInputMask();
        void recalculateOutputMask();
        std::vector<Pin> parents;
        std::vector<std::vector<Pin>> children;
    protected:
        uint32_t inputMask = 0;
        uint32_t outputMask = 0;
    };

    void connect(Pin parent, Pin child);

    void disconnect(Pin parent, Pin child);

    void update(std::queue<Node*>* queue, Node* node);
}

#endif //BUILDIT_S_NODE_H
