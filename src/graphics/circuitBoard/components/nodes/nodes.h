//
// Created by felix on 8/28/24.
//

#ifndef BUILDIT_NODES_H
#define BUILDIT_NODES_H

#include "nodeContainer.h"

/**
 * Handles various Nodes and their pins incl. movement of Nodes.
 */
class Nodes : public NodeContainer {
private:
    void removePins(Node* node);
    void addPins(Node* node);
    void updatePins();
    void updateNodePins(Node* node, glm::vec2 newCell);
    void updatePinCell(glm::vec2 oldCell, glm::vec2 newCell);
public:
    Nodes();
    std::unordered_map<glm::vec2, std::shared_ptr<Node>> nodes;
    std::unordered_map<glm::vec2, Node*> inputPins; // All input Pins formatted as cells
    std::unordered_map<glm::vec2, Node*> outputPins; // All output Pins formatted as cells
    std::vector<glm::vec2> pins; // All Pins of all Nodes but formatted as position, not pos! (*32)
    InstancedVertexRenderer pinRenderer{};
    void moveNode(Node* node, glm::vec2 newPos, bool updateBuffer) override;
    void addNode(const std::shared_ptr<Node>& node) override;
    void removeNode(Node* node) override;
    bool isOccupied(glm::vec2 cell, std::unordered_set<Node*> ignored);
    std::shared_ptr<Node> getNode(glm::vec2 cell);
};


#endif //BUILDIT_NODES_H
