//
// Created by felix on 9/1/24.
//

#ifndef BUILDIT_NODEPINS_H
#define BUILDIT_NODEPINS_H


#include "node.h"
#include "glm/vec2.hpp"

class NodePins : public MultiObserver<MoveEvent, Node*>, public MultiObserver<RotateEvent, Node*> {
private:
    void removePins(Node* node);
    void addPins(Node* node);
    void updatePins();
    void updateNodePins(Node* node, glm::vec2 newPos);
    void updatePinPos(glm::vec2 oldPos, glm::vec2 newPos);

    std::unordered_map<glm::vec2, Node*> inputPins;
    std::unordered_map<glm::vec2, Node*> outputPins;
    std::vector<glm::vec2> pins;
    InstancedVertexRenderer pinRenderer{};
public:
    void addNode(Node* node);
    void removeNode(Node* node);
    void update(const MoveEvent& event, Node *node) override;
    void update(const RotateEvent& event, Node *node) override;

    ~NodePins() override;
};


#endif //BUILDIT_NODEPINS_H
