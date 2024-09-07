//
// Created by felix on 9/1/24.
//

#ifndef BUILDIT_ROTATABLE_H
#define BUILDIT_ROTATABLE_H


#include "graphics/circuitBoard/observer.h"

template <typename T>
struct RotateEvent {
    float newRotation;
    float delta;
    bool before;
};

/**
 * Marks a component as rotatable
 */
template <typename T>
class Rotatable : public Subject<RotateEvent<T>> {
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

template <typename T>
float Rotatable<T>::getRotation() const {
    return this->rotation;
}

template <typename T>
void Rotatable<T>::rotate(float newRotation) {
    this->notify(RotateEvent<T>{newRotation, newRotation - rotation, true});
    this->rotation = newRotation;
    this->notify(RotateEvent<T>{newRotation, newRotation - rotation, false});
}

template <typename T>
Rotatable<T>::Rotatable(float rotation) {
    this->rotation = rotation;
}

#endif //BUILDIT_ROTATABLE_H
