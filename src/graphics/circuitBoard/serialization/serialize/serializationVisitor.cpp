//
// Created by felix on 9/27/24.
//

#include "serializationVisitor.h"

#include <utility>

SerializationVisitor::SerializationVisitor(std::unordered_set<std::shared_ptr<Component>>* components,
                                           std::queue<Sim::Node*> updateQueue)
    : updateQueue(std::move(updateQueue)), components(components) {

}
