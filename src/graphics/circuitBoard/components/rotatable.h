//
// Created by felix on 9/1/24.
//

#ifndef BUILDIT_ROTATABLE_H
#define BUILDIT_ROTATABLE_H


#include "graphics/circuitBoard/observer.h"

struct RotateEvent {
    float newRotation;
    float delta;
};

/**
 * Marks a component as rotatable
 */
class Rotatable : public Subject<RotateEvent> {
private:
    /**
     * Rotation in radians
     */
    float rotation = 0;
public:
    Rotatable() = default;
    explicit Rotatable(float rotation);
    [[nodiscard]] float getRotation() const;
    virtual void rotate(float newRotation);
};

#endif //BUILDIT_ROTATABLE_H
