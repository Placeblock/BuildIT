//
// Created by felix on 25.04.25.
//

#ifndef PINCACHE_HPP
#define PINCACHE_HPP

#include "components.hpp"

namespace Models {
    class PinRenderCache {
        struct CachedPin {
            PinSink *pin;
            Position position;
        };

        flecs::observer observer;
        std::vector<CachedPin> pins;
    public:
        void use(const flecs::world &world);

        void addPin(PinSink *pin, const Position& pos);
        void removePin(PinSink *pin);
        void updatePin(PinSink *pin, const Position& pos);
    };
}



#endif //PINCACHE_HPP
