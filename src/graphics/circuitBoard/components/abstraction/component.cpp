//
// Created by felix on 9/17/24.
//

#include "component.h"

#include <utility>

std::string Component::getUniqueID() {
    return this->cnamespace + this->ckey;
}

Component::Component(std::string cnamespace, std::string ckey)
    : cnamespace(std::move(cnamespace)), ckey(std::move(ckey)) {

}
