//
// Created by felix on 8/2/24.
//

#ifndef BUILDIT_WIRE_H
#define BUILDIT_WIRE_H

#include <iostream>
#include <set>
#include <list>

#include "glm/vec3.hpp"
#include "graphics/types.h"
#include "pin.h"
#include "graphics/circuitBoard/components/abstraction/movable.h"
#include "graphics/circuitBoard/components/abstraction/circleInteractable.h"
#include "graphics/circuitBoard/components/abstraction/lineInteractable.h"
#include "graphics/circuitBoard/selection/selectable.h"
#include "simulation/simulation.h"
#include "graphics/circuitBoard/events/networkEvents.h"
#include "graphics/circuitBoard/components/cabling/network.h"

class Wire final : public Networkable, public Movable, public Selectable, public LineInteractable {
protected:
    [[nodiscard]] glm::vec2 getStartPos() const override;
    [[nodiscard]] glm::vec2 getEndPos() const override;
public:
    Wire(Joint* start, Joint* end);
    Wire(Joint* start, Joint* end, std::shared_ptr<Network> network);
    Wire(Wire& other);

    Joint* start = nullptr;
    Joint* end = nullptr;
    [[nodiscard]] Joint* getOther(const Joint* cell) const;

	void connect();
	void disconnect();

    void serialize(SerializationContext &context) override;

    [[nodiscard]] Color getColor() const;
};


#endif //BUILDIT_WIRE_H
