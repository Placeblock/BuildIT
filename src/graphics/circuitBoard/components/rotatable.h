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
template<typename S>
class Rotatable : public Subject<RotateEvent, S> {
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

template<typename S>
float Rotatable<S>::getRotation() const {
    return this->rotation;
}

template<typename S>
void Rotatable<S>::rotate(float newRotation) {
    this->notify(this->getSubject(), RotateEvent{newRotation, newRotation - rotation});
    this->rotation = newRotation;
}

template<typename S>
Rotatable<S>::Rotatable(float rotation) {
    this->rotation = rotation;
}

#endif //BUILDIT_ROTATABLE_H
