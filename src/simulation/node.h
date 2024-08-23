//
// Created by felix on 13.07.24.
//

#ifndef BUILDIT_S_NODE_H
#define BUILDIT_S_NODE_H

#include <cstdint>
#include <memory>
#include <vector>
#include <queue>
#include "nodeUpdater.h"

namespace Sim {
    class Node;

    class Reference {
    public:
        std::shared_ptr<Node> node;
        std::shared_ptr<Node> targetNode;
        uint8_t index;
        bool operator==(const Reference& other) const {
            return (node == other.node) && (targetNode == other.targetNode) && (index == other.index);
        }
    };

    class Node {
    public:
        Node(uint8_t inputs, uint8_t outputs);
        virtual void update() = 0;
        uint32_t input = 0;
        uint32_t output = 0;
        void setInput(uint8_t index, bool value);
        void setOutput(uint8_t index, bool value);
        [[nodiscard]] bool getInput(uint8_t index) const;
        [[nodiscard]] bool getOutput(uint8_t index) const;
        void recalculateInputMask();
        void recalculateOutputMask();
        std::vector<Reference> parents;
        std::vector<std::vector<Reference>> children;
        std::vector<std::string> inputNames;
        std::vector<std::string> outputNames;
        NodeUpdater* updater = nullptr;
    protected:
        uint32_t inputMask = 0;
        uint32_t outputMask = 0;
    };

    void update(std::queue<std::shared_ptr<Node>>* queue, std::shared_ptr<Node> node);
}

#endif //BUILDIT_S_NODE_H