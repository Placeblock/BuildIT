//
// Created by felix on 9/27/24.
//

#ifndef BUILDIT_BINARYSERIALIZATIONVISITOR_H
#define BUILDIT_BINARYSERIALIZATIONVISITOR_H

#include <unordered_map>

#include "serializationVisitor.h"
#include "graphics/circuitBoard/serialization/counter.h"
#include "graphics/circuitBoard/serialization/identification.h"

class Network;

const uint COMPONENT_ID_SIZE = 2; // Size in Bytes

class BinarySerializationVisitor : public SerializationVisitor {

    Identificator<Network> networkIDs{};
    Identificator<Joint> jointIDs{};
    Identificator<Sim::Node> nodeIDs{};

    void doFor(NotGate *notGate) override;
    void doFor(Joint *joint) override;
    void doFor(Wire *wire) override;
public:
    explicit BinarySerializationVisitor(std::unordered_set<std::shared_ptr<Component>>* components, std::queue<Sim::Node*> updateQueue);

    void serialize() override;

};


#endif //BUILDIT_BINARYSERIALIZATIONVISITOR_H
