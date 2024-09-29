//
// Created by felix on 9/29/24.
//

#ifndef BUILDIT_JOINT_H
#define BUILDIT_JOINT_H


#include "network.h"
#include "graphics/circuitBoard/components/abstraction/movable.h"
#include "graphics/circuitBoard/selection/selectable.h"
#include "graphics/circuitBoard/components/abstraction/circleInteractable.h"

class Wire;

class Joint final : public Networkable, public Movable, public Selectable, public CircleInteractable {
private:
    glm::vec2 pos;
protected:
    [[nodiscard]] glm::vec2 getCenter() const override;
public:
    std::set<Wire*> wires;
    Pin pin{};

    explicit Joint(glm::vec2 pos);
    Joint(glm::vec2 pos, std::shared_ptr<Network> network);
    Joint(Joint& other);

    [[nodiscard]] Wire* getWire(Joint* other) const;
    void onMove(glm::vec2 delta) override;

    [[nodiscard]] glm::vec2 getPos() const;
    [[nodiscard]] Color getColor() const;
};


#endif //BUILDIT_JOINT_H
