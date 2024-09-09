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

class NodePins : public Observer<MoveEvent>, public Observer<RotateEvent>,
                 public Observer<ComponentAddEvent>, public Observer<ComponentRemoveEvent>, public NodePinHandler {
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
    void notify(Subject<MoveEvent> *subject, const MoveEvent& event) override;
    void notify(Subject<RotateEvent> *subject, const RotateEvent& event) override;
    void notify(Subject<ComponentAddEvent> *subject, const ComponentAddEvent& data) override;
    void notify(Subject<ComponentRemoveEvent> *subject, const ComponentRemoveEvent& data) override;

    bool isInputPin(glm::vec2 pos) override;
    bool isOutputPin(glm::vec2 pos) override;
    bool isPin(glm::vec2 pos) override;
    Node* getNode(glm::vec2 pos) override;

    void render(Program *program);
};


#endif //BUILDIT_NODEPINS_H
