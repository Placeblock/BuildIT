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
#include "wireContainer.h"
#include "networkContainer.h"

/**
 * Handles wires and joints and their movement.
 */
class Cabling : public Observer<MoveEvent>,
                public Observer<ComponentAddEvent>, public Observer<ComponentRemoveEvent> {
private:
    std::unordered_map<glm::vec2, Joint*> posMap;
public:
    [[nodiscard]] Joint* getJoint(glm::vec2 pos) const;

    static void setNetwork(Joint *joint, Network *network);
    static void setNetwork(Wire *wire, Network *network);

    void notify(const MoveEvent& event) override;
    void notify(const ComponentAddEvent& data) override;
    void notify(const ComponentRemoveEvent& data) override;
};


#endif //BUILDIT_CABLING_H
