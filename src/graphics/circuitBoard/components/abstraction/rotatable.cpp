//
// Created by felix on 9/6/24.
//

#include "rotatable.h"


float Rotatable::getRotation() const {
    return this->rotation;
}

void Rotatable::rotate(float newRotation) {
    this->notify({this, newRotation, newRotation - rotation, true});
    this->rotation = newRotation;
    this->notify({this, newRotation, newRotation - rotation, false});
}

Rotatable::Rotatable(float rotation) {
    this->rotation = rotation;
}