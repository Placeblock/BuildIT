//
// Created by felix on 9/27/24.
//

#ifndef BUILDIT_BINARYDESERIALIZER_H
#define BUILDIT_BINARYDESERIALIZER_H

#include <unordered_map>
#include <memory>
#include "deserializer.h"
#include "simulation/node.h"

class Network;

class BinaryDeserializer : public Deserializer {
    std::unordered_map<uint32_t, std::shared_ptr<Network>> networks;
    std::unordered_map<uint32_t, Joint*> joints;
    std::unordered_map<uint32_t, Sim::Node*> nodes;
    std::unordered_map<uint32_t, std::unordered_set<Wire*>> waitingWiresStart;
    std::unordered_map<uint32_t, std::unordered_set<Wire*>> waitingWiresEnd;

    void deserializeNotGate(std::istream &in);
    void deserializeJoint(std::istream &in);
    void deserializeWire(std::istream &in);

    void checkWire(Wire *wire);

    std::shared_ptr<Network> getNetwork(uint32_t id);
public:
    void deserialize(std::istream &in) override;

};


#endif //BUILDIT_BINARYDESERIALIZER_H
