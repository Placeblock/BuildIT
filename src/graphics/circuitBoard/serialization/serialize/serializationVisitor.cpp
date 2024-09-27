//
// Created by felix on 9/27/24.
//

#include "serializationVisitor.h"

#include <utility>

SerializationVisitor::SerializationVisitor(std::unordered_set<Component *> components) : components(std::move(components)) {

}
