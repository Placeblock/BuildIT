//
// Created by felix on 8/28/24.
//

#ifndef BUILDIT_CABLING_H
#define BUILDIT_CABLING_H


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
class Cabling : Observer<MoveEvent<Joint>>,
                public Observer<JointAddEvent>, public Observer<JointRemoveEvent>,
                public Observer<WireAddEvent>, public Observer<WireRemoveEvent> {
private:
    std::unordered_map<glm::vec2, Joint*> posMap;
public:
    Cabling(Subject<JointAddEvent> *jointAddObserver, Subject<JointRemoveEvent> *jointRemoveObserver,
            Subject<WireAddEvent> *wireAddObserver, Subject<WireRemoveEvent> *wireRemoveObserver);

    [[nodiscard]] Joint* getJoint(glm::vec2 pos) const;
    Wire* getWire(glm::vec2 pos);

    static void setNetwork(Joint *joint, Network *network);
    static void setNetwork(Wire *wire, Network *network);

    void update(Subject<MoveEvent<Joint>> *subject, const MoveEvent<Joint>& event) override;
    void update(Subject<JointAddEvent> *subject, const JointAddEvent& data) override;
    void update(Subject<JointRemoveEvent> *subject, const JointRemoveEvent& data) override;
    void update(Subject<WireAddEvent> *subject, const WireAddEvent& data) override;
    void update(Subject<WireRemoveEvent> *subject, const WireRemoveEvent& data) override;
};


#endif //BUILDIT_CABLING_H
