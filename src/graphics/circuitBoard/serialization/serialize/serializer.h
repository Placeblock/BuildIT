//
// Created by felix on 9/29/24.
//

#ifndef BUILDIT_SERIALIZER_H
#define BUILDIT_SERIALIZER_H

#include <unordered_set>
#include <memory>
#include <queue>
#include "serializationContext.h"

class Component;

class Serializer {
public:
    SerializationContext context;
    Identificator<std::string> keyIDs;
    void serialize(std::unordered_set<std::shared_ptr<Component>>* components,
                   std::queue<Sim::Node*> updateQueue);

};


#endif //BUILDIT_SERIALIZER_H
