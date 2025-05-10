//
// Created by felix on 11.05.25.
//

#include <utility>

#include "../../include/history/event.hpp"

Event::Event(std::string type, std::string name, std::string description, Player *player)
    : type(std::move(type)), name(std::move(name)), description(std::move(description)), player(player) {
}
