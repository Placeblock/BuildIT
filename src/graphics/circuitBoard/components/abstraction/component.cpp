//
// Created by felix on 9/17/24.
//

#include "component.h"

#include <utility>

std::string Component::getUniqueID() const {
    return this->cnamespace + this->ckey;
}

Component::Component(std::string cnamespace, std::string ckey)
    : cnamespace(std::move(cnamespace)), ckey(std::move(ckey)) {
}

Component::Component(const Component &other) = default;

std::string Component::getNamespace() const {
    return this->cnamespace;
}

std::string Component::getKey() const {
    return this->ckey;
}