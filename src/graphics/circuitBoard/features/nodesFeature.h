//
// Created by felix on 8/31/24.
//

#ifndef BUILDIT_NODESFEATURE_H
#define BUILDIT_NODESFEATURE_H

#include "feature.h"
#include "graphics/circuitBoard/components/nodes/nodePins.h"
#include "graphics/circuitBoard/components/componentContainer.h"

/**
 * Adds functionality for rendering nodes on the CircuitBoard
 */
class NodesFeature final : public Feature, public Renderable,
        public Observer<ComponentAddEvent>, public Observer<ComponentRemoveEvent> {
    NodePins nodePins;
private:
    std::unordered_set<Node*> nodes;
public:
    explicit NodesFeature(Programs *programs, Subject<ComponentAddEvent> *addSubject, Subject<ComponentRemoveEvent> *removeSubject);

    NodePinHandler *getNodePinHandler();
    void render() override;

    void notify(const ComponentAddEvent& event) override;
    void notify(const ComponentRemoveEvent& event) override;

    std::unordered_set<Node*>* getNodes();
};


#endif //BUILDIT_NODESFEATURE_H
