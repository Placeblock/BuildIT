//
// Created by felix on 02.05.25.
//

#ifndef OBSERVER_HPP
#define OBSERVER_HPP
#include "flecs/addons/cpp/flecs.hpp"

flecs::observer updateSimulationLinks(const flecs::world &world);
flecs::observer synchronizeConnectedPair(const flecs::world &world);

#endif //OBSERVER_HPP
