//
// Created by felix on 25.04.25.
//

#ifndef PINCACHE_HPP
#define PINCACHE_HPP
#include <unordered_map>

#include "components.hpp"

namespace Models {
    class PinCache {
        flecs::observer observer;
        std::unordered_map<Position, Pin> pins;

    public:
        void use(const flecs::world &world);
    };
}



#endif //PINCACHE_HPP
