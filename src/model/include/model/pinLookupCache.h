//
// Created by felix on 27.04.25.
//

#ifndef PINLOOKUPCACHE_H
#define PINLOOKUPCACHE_H
#include <unordered_map>

#include "components.hpp"


namespace Models {

    class PinLookupCache {
        flecs::observer observer;
        std::unordered_map<Position, Pin*> pins;
    public:
        void use(const flecs::world &world);

        void addPin(Pin *pin, const Position& pos);
        void removePin(Pin *pin);
        void updatePin(Pin *pin, const Position& pos);

        Pin* getPin(const Position& pos) const;
    };
}


#endif //PINLOOKUPCACHE_H
