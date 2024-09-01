//
// Created by felix on 9/1/24.
//

#include "rotatable.h"

float Rotatable::getRotation() const {
    return this->rotation;
}

void Rotatable::rotate(float newRotation) {
    this->notify(RotateEvent{newRotation, newRotation - rotation});
    this->rotation = newRotation;
}

Rotatable::Rotatable(float rotation) {
    this->rotation = rotation;
}
