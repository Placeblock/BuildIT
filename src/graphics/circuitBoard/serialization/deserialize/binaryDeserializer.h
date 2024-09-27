//
// Created by felix on 9/27/24.
//

#ifndef BUILDIT_BINARYDESERIALIZER_H
#define BUILDIT_BINARYDESERIALIZER_H

#include <unordered_map>
#include <memory>
#include "deserializer.h"

class Network;

class BinaryDeserializer : public Deserializer {

    std::unordered_map<uint32_t, std::shared_ptr<Network>> networks;
    std::unordered_map<uint32_t, Joint*> joints;
    std::unordered_map<uint32_t, std::unordered_set<Wire*>> waitingWiresStart;
    std::unordered_map<uint32_t, std::unordered_set<Wire*>> waitingWiresEnd;

    void deserialize(std::istream &in) override;

    void deserializeNotGate(std::istream &in);
    void deserializeJoint(std::istream &in);
    void deserializeWire(std::istream &in);

    std::shared_ptr<Network> getNetwork(uint32_t id);

};


#endif //BUILDIT_BINARYDESERIALIZER_H
