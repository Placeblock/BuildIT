//
// Created by felix on 8/28/24.
//

#ifndef BUILDIT_NODES_H
#define BUILDIT_NODES_H

#include "nodeContainer.h"
#include <memory>

/**
 * Handles various nodes
 */
class Nodes : public MultiObserver<MoveEvent, Node*> {
private:
    std::unordered_map<glm::vec2, Node*> nodeMap;
public:
    void addNode(Node* node);
    void removeNode(Node* node);
    void update(const MoveEvent& event, Node *node) override;

    bool isOccupied(glm::vec2 pos, std::unordered_set<Node*> ignored);
    Node *getIntersectedNode(glm::vec2 pos);

    ~Nodes() override;
};


#endif //BUILDIT_NODES_H
