//
// Created by felix on 11.05.25.
//

#include "event/eventtarget.hpp"

using namespace buildit::event;

void event_target_manager::add(const std::string &name, std::unique_ptr<event_target> target) {
    targets[name] = std::move(target);
}

void event_target_manager::remove(const std::string &name) {
    targets.erase(name);
}
