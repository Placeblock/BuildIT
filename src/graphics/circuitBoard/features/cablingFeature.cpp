//
// Created by felix on 8/31/24.
//

#include "cablingFeature.h"

#include "graphics/circuitBoard/history/actions/createComponentAction.h"
#include "graphics/circuitBoard/components/nodes/node.h"

CablingFeature::CablingFeature(History *history,
                               ComponentContainer *componentContainer,
                               CablingRenderer *cablingRenderer)
                               : history(history), componentContainer(componentContainer), cablingRenderer(cablingRenderer) {
    componentContainer->Subject<ComponentAddEvent>::subscribe(this);
    componentContainer->Subject<ComponentRemoveEvent>::subscribe(this);
    componentContainer->Subject<ComponentAddEvent>::subscribe(&this->cabling);
    componentContainer->Subject<ComponentRemoveEvent>::subscribe(&this->cabling);
}

void CablingFeature::notify(const ComponentRemoveEvent &data) {
    if (const auto *joint = dynamic_cast<Joint*>(data.component)) {
        for (const auto &wire: joint->wires) {
            std::shared_ptr<Wire> owningRef = std::dynamic_pointer_cast<Wire>(this->componentContainer->getOwningRef(wire));
            std::unique_ptr<Action> dAction = std::make_unique<CreateComponentAction>(this->componentContainer,
                                                                                      owningRef,
                                                                                      true);
            History::dispatch(this->history, dAction);
        }
        if (joint->getNetwork()->wires.empty() && joint->getNetwork()->joints.empty()) {
            this->networks.removeNetwork(joint->getNetwork());
        }
    } else if (const auto *wire = dynamic_cast<Wire*>(data.component)) {
        std::set<Joint*> resolverData;
        resolverData.insert(wire->start);
        resolverData.insert(wire->end);
        NetworkResolver resolver{resolverData};
        resolver.resolve();
        if (resolver.resolved.size() > 1) { // The new Network was split

            const auto newNetwork = std::make_shared<Network>();  // The new newNetwork
            this->networks.addNetwork(newNetwork);

            const bool moveParentRef = resolver.resolved[1].contains(wire->getNetwork()->parentPin.first);

            // The vertex of the new network was a parent reference, so we have to move the reference and disconnect output references
            if (moveParentRef) {
                newNetwork->parentPin = wire->getNetwork()->parentPin;
                newNetwork->parentPin.second.node->outputNetworks[newNetwork->parentPin.second.index] = newNetwork.get();
            }

            for (const auto &joint: resolver.resolved[1]) { // Update
                // The wire was a child reference, so we move the reference and disconnect it from the parent reference if it won't get moved
                if (joint->getNetwork()->childPins.contains(joint)) {
                    const auto childRef = joint->getNetwork()->childPins[joint];
                    newNetwork->childPins[joint] = childRef;
                    joint->getNetwork()->childPins.erase(joint);
                }
                joint->getNetwork()->removeJoint(joint);
                newNetwork->joints.push_back(joint);

                joint->setNetwork(newNetwork.get());
                for (const auto &jointWire: joint->wires) {
                    jointWire->getNetwork()->removeWire(jointWire);
                    newNetwork->wires.push_back(jointWire);
                    jointWire->setNetwork(newNetwork.get());
                }
            }
            if (moveParentRef) {
                wire->getNetwork()->parentPin = {};
            }

            const std::unordered_set newNetworks{wire->getNetwork(), newNetwork.get()};
            this->Subject<NetworksSplitEvent>::notify({wire->getNetwork(), newNetworks});
        }
    }
}

void CablingFeature::notify(const ComponentAddEvent &data) {
    if (auto *wire = dynamic_cast<Wire*>(data.component)) {
        wire->setNetwork(wire->start->getNetwork());
        if (wire->start->getNetwork() != wire->end->getNetwork()) { // We have to merge networks
            Network *deletedNetwork = wire->end->getNetwork();
            this->Subject<NetworksMergeEvent>::notify(NetworksMergeEvent{wire->start->getNetwork(), deletedNetwork});

            // We add all the old output References to the merged network
            for (auto &[oldChildJoint, oldChildPin] : deletedNetwork->childPins) {
                wire->getNetwork()->childPins[oldChildJoint] = oldChildPin;
            }
            // Change networks
            for (const auto &joint: deletedNetwork->joints) {
                joint->setNetwork(wire->getNetwork());
                wire->getNetwork()->joints.push_back(joint);
            }
            for (const auto &delWire: deletedNetwork->wires) {
                delWire->setNetwork(wire->getNetwork());
                wire->getNetwork()->wires.push_back(delWire);
            }
            // We merge the deleted input reference into the merged network if necessary
            if (wire->getNetwork()->parentPin.first == nullptr
                && deletedNetwork->parentPin.first != nullptr) {
                wire->getNetwork()->parentPin = deletedNetwork->parentPin;
                deletedNetwork->parentPin.second.node->outputNetworks[deletedNetwork->parentPin.second.index] = wire->getNetwork();
                // We update the new network if the parentPin from the deleted network was merged
                this->cablingRenderer->updateNetwork(wire->getNetwork());
            }
            // We don't remove the wires and jointVertexData from the old network to support rewind easily
            this->networks.removeNetwork(deletedNetwork);
        }
    }
}
