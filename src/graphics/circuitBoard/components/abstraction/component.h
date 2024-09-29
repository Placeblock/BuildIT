//
// Created by felix on 9/1/24.
//

#ifndef BUILDIT_COMPONENT_H
#define BUILDIT_COMPONENT_H

#include "graphics/circuitBoard/serialization/serialize/serializationContext.h"

class Component {
    std::string cnamespace;
    std::string ckey;
public:
    Component(std::string cnamespace, std::string ckey);
    Component(const Component &other);
    [[nodiscard]] std::string getUniqueID() const;
    [[nodiscard]] std::string getNamespace() const;
    [[nodiscard]] std::string getKey() const;
    virtual void serialize(SerializationContext &context) = 0;

    virtual ~Component() = default;
};


#endif //BUILDIT_COMPONENT_H
