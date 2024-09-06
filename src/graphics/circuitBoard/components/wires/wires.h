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
class Cabling : MultiObserver<MoveEvent, Joint*>,
                public Observer<JointAddEvent>, public Observer<JointRemoveEvent>,
                public Observer<WireAddEvent>, public Observer<WireRemoveEvent> {
private:
    std::unordered_map<intVec2, Joint*> cellMap;
    std::unordered_map<Wire*, Network*> wireMap;
public:
    [[nodiscard]] Joint* getJoint(intVec2 cell) const;
    Wire* getWire(glm::vec2 cell);

    static void setNetwork(Joint *joint, Network *network);
    void setNetwork(Wire *wire, Network *network);

    void update(const MoveEvent& event, Joint *joint) override;
    void update(const JointAddEvent& data) override;
    void update(const JointRemoveEvent& data) override;
    void update(const WireAddEvent& data) override;
    void update(const WireRemoveEvent& data) override;

};


#endif //BUILDIT_WIRES_H
