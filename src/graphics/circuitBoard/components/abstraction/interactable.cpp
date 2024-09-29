//
// Created by felix on 29.09.24.
//

#include "interactable.h"

#include <utility>

Interactable::Interactable(std::string cnamespace, std::string ckey) : Component(std::move(cnamespace), std::move(ckey)) {

}

Interactable::Interactable(const Interactable &other) = default;