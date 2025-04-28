//
// Created by felix on 28.04.25.
//

#ifndef MOVEEVENTHANDLER_H
#define MOVEEVENTHANDLER_H

#include <functional>

#include "events/event.hpp"
#include "flecs/addons/cpp/world.hpp"

std::function<void(const Events::MoveEvent&)> moveEventHandler(flecs::world &world);



#endif //MOVEEVENTHANDLER_H
