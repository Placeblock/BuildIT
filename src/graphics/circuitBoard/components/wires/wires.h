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
class Wires : public JointContainer, public WireContainer, public NetworkContainer {
public:
    std::unordered_set<std::shared_ptr<Network>> networks;
    std::set<std::shared_ptr<Joint>> joints;
    std::set<std::shared_ptr<Wire>> wires;
    std::unordered_map<intVec2, Joint*> cellMap;
    std::unordered_map<Joint*, Network*> jointMap;
    std::unordered_map<Wire*, Network*> wireMap;
    [[nodiscard]] Joint* getJoint(intVec2 cell) const;
    Wire* getWire(glm::vec2 pair);
    Network* getNetwork(Joint* joint);
    void removeJoint(Joint* joint) override;
    void removeWire(Wire* wire) override;
    void addJoint(const std::shared_ptr<Joint>& joint) override;
    void addWire(const std::shared_ptr<Wire>& wire) override;
    void moveJoint(Joint* joint, glm::vec2 newCell) override;
    void addNetwork(const std::shared_ptr<Network>& network) override;
    void removeNetwork(Network *network) override;
    [[nodiscard]] size_t getJointIndex(const Joint* joint) const override;
    [[nodiscard]] size_t getWireIndex(const Wire* wire) const override;

    [[nodiscard]] std::shared_ptr<Joint> getOwningRef(const Joint* joint) const;
    [[nodiscard]] std::shared_ptr<Wire> getOwningRef(const Wire* wire) const;
    [[nodiscard]] std::shared_ptr<Network> getOwningRef(const Network* network) const override;

    std::set<const Joint*> getJoints() const override;
    std::set<const Wire*> getWires() const override;
};


#endif //BUILDIT_WIRES_H
