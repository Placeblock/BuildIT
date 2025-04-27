//
// Created by felix on 25.04.25.
//

#ifndef PINCACHE_HPP
#define PINCACHE_HPP
#include <unordered_map>

#include "components.hpp"

namespace Models {
    class PinCache {
        struct CachedPin {
            Pin *pin;
            Position position;
        };

        flecs::observer observer;
        std::vector<CachedPin> pins;
    public:
        void use(const flecs::world &world);

        void addPin(Pin *pin, const Position& pos);
        void removePin(Pin *pin);
        void updatePin(Pin *pin, const Position& pos);
    };
}



#endif //PINCACHE_HPP
