//
// Created by felix on 13.07.24.
//

#ifndef BUILDIT_S_NODE_H
#define BUILDIT_S_NODE_H

#include <cstdint>
#include <string>
#include <vector>
#include <queue>

namespace Sim {
    class Node;

    class Reference {
    public:
        Node* node;
        Node* targetNode;
        uint8_t index;
        bool operator==(const Reference& other) const {
            return (node == other.node) && (targetNode == other.targetNode) && (index == other.index);
        }
    };

    class Updater {
    public:
        virtual uint32_t update(uint32_t input, uint32_t inputMask, uint32_t outputMask) = 0;
    };

    class Node {
    public:
        Node(uint8_t inputs, uint8_t outputs, Updater *updater);
        Node(Node& other);

        void update();
        void setInput(uint8_t index, bool value);
        [[nodiscard]] bool getInput(uint8_t index) const;
        [[nodiscard]] bool getOutput(uint8_t index) const;
        [[nodiscard]] uint32_t getOutput() const;
        void recalculateInputMask();
        void recalculateOutputMask();
        std::vector<Reference> parents;
        std::vector<std::vector<Reference>> children;
        bool updated = false;
    private:
        Updater *updater;

        uint32_t inputMask = 0;
        uint32_t outputMask = 0;

        uint32_t input = 0;
        uint32_t output = 0;

        std::vector<std::string> inputNames;
        std::vector<std::string> outputNames;
    };

    void update(std::queue<Node*>* queue, Node* node);
}

#endif //BUILDIT_S_NODE_H