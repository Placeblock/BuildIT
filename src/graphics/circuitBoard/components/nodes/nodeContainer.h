//
// Created by felix on 8/28/24.
//

#ifndef BUILDIT_NODECONTAINER_H
#define BUILDIT_NODECONTAINER_H

#include <memory>
#include "glm/glm.hpp"
#include "node.h"

class NodeContainer : public Subject<ComponentAddEvent>, public Subject<ComponentRemoveEvent> {
private:
    std::list<std::shared_ptr<Node>> nodes;
public:
    void addNode(const std::shared_ptr<Node>& node);
    void removeNode(Node* node);
    [[nodiscard]] const std::list<std::shared_ptr<Node>> *getNodes() const;
};

#endif //BUILDIT_NODECONTAINER_H
