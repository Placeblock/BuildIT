//
// Created by felix on 8/28/24.
//

#ifndef BUILDIT_NODES_H
#define BUILDIT_NODES_H

#include "nodeContainer.h"

/**
 * Handles various Nodes and their pins incl. movement of Nodes.
 */
class Nodes : public NodeContainer, Observer<MoveEvent, Node> {
private:
    std::unordered_set<std::shared_ptr<Node>> nodes;
    std::unordered_map<glm::vec2, Node*> nodeMap;
public:
    void addNode(const std::shared_ptr<Node>& node) override;
    void removeNode(Node* node) override;
    void update(Node *node, const MoveEvent& event) override;

    bool isOccupied(glm::vec2 pos, std::unordered_set<Node*> ignored);
    Node *getIntersectedNode(glm::vec2 pos);

    ~Nodes() override;
};


#endif //BUILDIT_NODES_H
