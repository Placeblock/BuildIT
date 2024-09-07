//
// Created by felix on 8/28/24.
//

#ifndef BUILDIT_NODES_H
#define BUILDIT_NODES_H

#include "nodeContainer.h"
#include <memory>

/**
 * Handles various joints
 */
class NodeInteractionManager : public Observer<MoveEvent<Node>>,
        public Observer<NodeAddEvent>, public Observer<NodeRemoveEvent>  {
private:
    std::unordered_map<glm::vec2, Node*> nodeMap;
    Subject<NodeAddEvent> *nodeAddSubject;
    Subject<NodeAddEvent> *nodeRemoveSubject;
public:
    NodeInteractionManager(Subject<NodeAddEvent> *nodeAddSubject, Subject<NodeAddEvent> *nodeRemoveSubject);

    void update(Subject<MoveEvent<Node>> *subject, const MoveEvent<Node>& event) override;
    void update(Subject<NodeAddEvent> *subject, const NodeAddEvent& data) override;
    void update(Subject<NodeRemoveEvent> *subject, const NodeRemoveEvent& data) override;

    bool isOccupied(glm::vec2 pos, std::unordered_set<Node*> ignored);
    Node *getIntersectedNode(glm::vec2 pos);
};


#endif //BUILDIT_NODES_H
