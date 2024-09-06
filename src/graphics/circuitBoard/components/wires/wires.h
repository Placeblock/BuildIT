//
// Created by felix on 8/28/24.
//

#ifndef BUILDIT_WIRES_H
#define BUILDIT_WIRES_H


#include <unordered_map>
#include <unordered_set>
#include <set>
#include <memory>
#include "wire.h"
#include "jointContainer.h"
#include "wireContainer.h"
#include "networkContainer.h"

/**
 * Handles wires and joints and their movement.
 */
class Wires : public WireContainer, public NetworkContainer, MultiObserver<MoveEvent, Joint*>,
          public Observer<JointAddEvent>, public Observer<JointRemoveEvent> {
private:
    std::unordered_set<std::shared_ptr<Network>> networks;
    std::set<std::shared_ptr<Wire>> wires;
    std::unordered_map<intVec2, Joint*> cellMap;
    std::unordered_map<Joint*, Network*> jointMap;
    std::unordered_map<Wire*, Network*> wireMap;
public:
    [[nodiscard]] Joint* getJoint(intVec2 cell) const;
    Wire* getWire(glm::vec2 cell) override;
    Network* getNetwork(Joint* joint);
    void removeWire(Wire* wire) override;
    void addWire(const std::shared_ptr<Wire>& wire) override;
    void addNetwork(const std::shared_ptr<Network>& network) override;
    void removeNetwork(Network *network) override;
    [[nodiscard]] size_t getWireIndex(const Wire* wire) const override;

    void setNetwork(Joint *joint, Network *network);
    void setNetwork(Wire *wire, Network *network) override;

    [[nodiscard]] std::shared_ptr<Wire> getOwningRef(const Wire* wire) const override;
    [[nodiscard]] std::shared_ptr<Network> getOwningRef(const Network* network) const override;

    std::set<const Wire*> getWires() const override;

    void update(const MoveEvent& event, Joint *joint) override;
    void update(const JointAddEvent& data) override;
    void update(const JointRemoveEvent& data) override;

};


#endif //BUILDIT_WIRES_H
