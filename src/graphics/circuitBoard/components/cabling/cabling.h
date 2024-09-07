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
class Cabling : public TypedObserver<MoveEvent, Joint>,
                public Observer<ComponentAddEvent>, public Observer<ComponentRemoveEvent>,
                public Observer<WireAddEvent>, public Observer<WireRemoveEvent> {
private:
    std::unordered_map<glm::vec2, Joint*> posMap;
public:
    Cabling(TypedSubject<ComponentAddEvent, Joint> *jointAddObserver, TypedSubject<ComponentRemoveEvent, Joint> *jointRemoveObserver,
            TypedSubject<ComponentAddEvent, Wire> *wireAddObserver, TypedSubject<ComponentRemoveEvent, Wire> *wireRemoveObserver);

    [[nodiscard]] Joint* getJoint(glm::vec2 pos) const;
    Wire* getWire(glm::vec2 pos);

    static void setNetwork(Joint *joint, Network *network);
    static void setNetwork(Wire *wire, Network *network);

    void notify(Joint *joint, const MoveEvent& event) override;
    void notify(Subject<ComponentAddEvent> *subject, const ComponentAddEvent& data) override;
    void notify(Subject<ComponentRemoveEvent> *subject, const ComponentRemoveEvent& data) override;
    void notify(Subject<WireAddEvent> *subject, const WireAddEvent& data) override;
    void notify(Subject<WireRemoveEvent> *subject, const WireRemoveEvent& data) override;
};


#endif //BUILDIT_CABLING_H
