//
// Created by felix on 9/1/24.
//

#ifndef BUILDIT_NODEPINS_H
#define BUILDIT_NODEPINS_H


#include "node.h"
#include "glm/vec2.hpp"
#include "nodeEvents.h"

class NodePinHandler {
public:
    virtual bool isInputPin(glm::vec2 pos) = 0;
    virtual bool isOutputPin(glm::vec2 pos) = 0;
    virtual bool isPin(glm::vec2 pos) = 0;
    virtual Node* getNode(glm::vec2 pos) = 0;
};

class NodePins : public MultiObserver<MoveEvent<Node>, Node*>, public MultiObserver<RotateEvent<Node>, Node*>,
public Observer<NodeAddEvent>, public Observer<NodeRemoveEvent>, public NodePinHandler {
private:
    void removePins(Node* node);
    void addPins(Node* node);
    void updatePins();
    void updateNodePins(Node* node, glm::vec2 newPos);
    void updatePinPos(glm::vec2 oldPos, glm::vec2 newPos);

    Subject<NodeAddEvent> *nodeAddSubject;
    Subject<NodeRemoveEvent> *nodeRemoveSubject;
    std::unordered_map<glm::vec2, Node*> inputPins;
    std::unordered_map<glm::vec2, Node*> outputPins;
    std::vector<glm::vec2> pins;
    InstancedVertexRenderer pinRenderer{};
public:
    NodePins(Subject<NodeAddEvent> *nodeAddSubject, Subject<NodeRemoveEvent> *nodeRemoveSubject);

    void update(const MoveEvent<Node>& event, Node *node) override;
    void update(const RotateEvent<Node>& event, Node *node) override;
    void update(const NodeAddEvent& data) override;
    void update(const NodeRemoveEvent& data) override;

    bool isInputPin(glm::vec2 pos) override;
    bool isOutputPin(glm::vec2 pos) override;
    bool isPin(glm::vec2 pos) override;
    Node* getNode(glm::vec2 pos) override;

    void render(Program *program);

    ~NodePins() override;
};


#endif //BUILDIT_NODEPINS_H
