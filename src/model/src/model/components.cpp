//
// Created by felix on 23.04.25.
//

#include "model/components.hpp"

Models::Joint * Models::Wire::getOther(const Joint *joint) const  {
    return joint == this->left ? this->right : this->left;
}
