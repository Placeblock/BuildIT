//
// Created by felix on 9/1/24.
//

#ifndef BUILDIT_ROTATABLE_H
#define BUILDIT_ROTATABLE_H

#include "graphics/circuitBoard/observer.h"

#include "graphics/circuitBoard/components/visitor.h"
#include "component.h"

class Rotatable;

struct RotateEvent {
    Rotatable *rotatable;
    float newRotation;
    float delta;
    bool before;
};

/**
 * Marks a component as rotatable
 */
class Rotatable : public Subject<RotateEvent>, virtual public Component {
private:
    /**
     * Rotation in radians
     */
    float rotation = 0;
public:
    explicit Rotatable(float rotation);
    Rotatable() = default;

    [[nodiscard]] float getRotation() const;
    virtual void rotate(float newRotation);
};

#endif //BUILDIT_ROTATABLE_H
