//
// Created by felix on 11.05.25.
//

#include <utility>

#include "../../include/history/event.hpp"

Event::Event(std::string type, const Player *player)
    : type(std::move(type)), player(player) {
}