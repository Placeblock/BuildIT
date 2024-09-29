//
// Created by felix on 9/1/24.
//

#ifndef BUILDIT_COMPONENT_H
#define BUILDIT_COMPONENT_H

#include <sstream>
#include "graphics/circuitBoard/components/renderer/renderer.h"

class SerializationContext;

class Component {
private:
    std::string cnamespace;
    std::string ckey;
public:
    Component(std::string cnamespace, std::string ckey);
    std::string getUniqueID();
    virtual void serialize(SerializationContext &context) = 0;
};


#endif //BUILDIT_COMPONENT_H
