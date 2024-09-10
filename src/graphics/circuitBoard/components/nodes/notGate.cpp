//
// Created by felix on 9/10/24.
//

#include "notGate.h"


void NotGate::visit(Visitor *visitor) {
    visitor->doFor(this);
}