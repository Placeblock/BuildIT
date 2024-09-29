//
// Created by felix on 9/29/24.
//

#ifndef BUILDIT_SERIALIZATIONCONTEXT_H
#define BUILDIT_SERIALIZATIONCONTEXT_H

#include <sstream>
#include "graphics/circuitBoard/serialization/identification.h"

class Network;
class Joint;
namespace Sim {
    class Node;
}

struct SerializationContext {
    std::stringstream serialized;

    Identificator<Network*> networkIDs{};
    Identificator<Joint*> jointIDs{};
    Identificator<Sim::Node*> nodeIDs{};
};


#endif //BUILDIT_SERIALIZATIONCONTEXT_H
