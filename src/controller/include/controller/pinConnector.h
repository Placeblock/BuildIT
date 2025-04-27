//
// Created by felix on 27.04.25.
//

#ifndef PINCONNECTOR_H
#define PINCONNECTOR_H

#include "model/pinRenderCache.hpp"

namespace Controller {
    flecs::observer &pinConnector(const flecs::world& world, const Models::PinRenderCache& pinCache);
}


#endif //PINCONNECTOR_H
