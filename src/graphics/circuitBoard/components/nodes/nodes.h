//
// Created by felix on 8/28/24.
//

#ifndef BUILDIT_NODES_H
#define BUILDIT_NODES_H

#include <memory>
#include <unordered_map>
#include <unordered_set>

#include "glm/vec2.hpp"
#include "graphics/circuitBoard/components/abstraction/movable.h"
#include "graphics/circuitBoard/components/abstraction/component.h"
#include "glm/gtx/hash.hpp"


/**
 * Handles various joints
 */
class NodeInteractionManager : public Observer<MoveEvent>,
        public Observer<ComponentAddEvent>, public Observer<ComponentRemoveEvent>  {
private:
    std::unordered_map<glm::vec2, Node*> nodeMap;
public:
    void notify(Subject<MoveEvent> *subject, const MoveEvent& event) override;
    void notify(Subject<ComponentAddEvent> *subject, const ComponentAddEvent& data) override;
    void notify(Subject<ComponentRemoveEvent> *subject, const ComponentRemoveEvent& data) override;

    bool isOccupied(glm::vec2 pos, std::unordered_set<Node*> ignored);
    Node *getIntersectedNode(glm::vec2 pos);
};


#endif //BUILDIT_NODES_H
