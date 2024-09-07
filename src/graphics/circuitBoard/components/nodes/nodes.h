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
class NodeInteractionManager : public CastedObserver<MoveEvent, Node>,
        public Observer<ComponentAddEvent>, public Observer<ComponentRemoveEvent>  {
private:
    std::unordered_map<glm::vec2, Node*> nodeMap;
public:
    NodeInteractionManager(Subject<ComponentAddEvent> *nodeAddSubject, Subject<ComponentRemoveEvent> *nodeRemoveSubject);

    void notify(Node *node, const MoveEvent& event) override;
    void notify(Subject<ComponentAddEvent> *subject, const ComponentAddEvent& data) override;
    void notify(Subject<ComponentRemoveEvent> *subject, const ComponentRemoveEvent& data) override;

    bool isOccupied(glm::vec2 pos, std::unordered_set<Node*> ignored);
    Node *getIntersectedNode(glm::vec2 pos);
};


#endif //BUILDIT_NODES_H
