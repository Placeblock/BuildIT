//
// Created by felix on 8/21/24.
//

#ifndef BUILDIT_NODEDRAGHANDLER_H
#define BUILDIT_NODEDRAGHANDLER_H

class Node;

class NodeDragHandler {
public:
    virtual void setCreatingNode(std::unique_ptr<Node> node) = 0;

    virtual ~NodeDragHandler() = default;
};

#endif //BUILDIT_NODEDRAGHANDLER_H
