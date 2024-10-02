//
// Created by felix on 9/6/24.
//

#include "rotatable.h"

#include <utility>


float Rotatable::getRotation() const {
    return this->rotation;
}

void Rotatable::rotate(const float newRotation) {
    this->notify({this, newRotation, newRotation - rotation, true});
    this->rotation = newRotation;
    this->notify({this, newRotation, newRotation - rotation, false});
}

Rotatable::Rotatable(std::string cnamespace, std::string ckey, float rotation)
    : Component(std::move(cnamespace), std::move(ckey)) {
    this->rotation = rotation;
}

Rotatable::Rotatable(const Rotatable &other) : Component(other) {

}