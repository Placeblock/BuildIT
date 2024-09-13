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
                public Observer<ComponentAddEvent>, public Observer<ComponentRemoveEvent>,
                public Observer<WireAddEvent>, public Observer<WireRemoveEvent>{
private:
    std::unordered_set<Wire*> wires;
    std::unordered_map<intVec2 , Joint*> posMap;
public:
    [[nodiscard]] Joint* getJoint(intVec2 pos) const;
    Wire* getWire(glm::vec2 pos);

    void notify(const MoveEvent& event) override;
    void notify(const ComponentAddEvent& data) override;
    void notify(const ComponentRemoveEvent& data) override;
    void notify(const WireAddEvent& data) override;
    void notify(const WireRemoveEvent& data) override;
};


#endif //BUILDIT_CABLING_H
