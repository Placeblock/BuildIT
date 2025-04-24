//
// Created by felix on 12.04.25.
//

#ifndef NETWORK_HPP
#define NETWORK_HPP
#include <vector>

#include "components.hpp"

namespace Graph {
    struct Network {
        std::unordered_map<Id, std::unique_ptr<Wire>> wires;
        std::unordered_map<Id, std::unique_ptr<Joint>> joints;
    };
    
    class Graph {
        std::unordered_map<Id, Wire> wires;
        std::unordered_map<Id, Joint> joints;
    };
}

#endif //NETWORK_HPP
