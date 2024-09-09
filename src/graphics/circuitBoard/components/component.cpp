//
// Created by felix on 9/1/24.
//

#include "component.h"

void Component::visit(Visitor *visitor) {
    visitor->doFor(this);
}
