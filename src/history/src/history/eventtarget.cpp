//
// Created by felix on 11.05.25.
//

#include "history/eventtarget.hpp"


void EventTargetManager::add(const std::string &name, std::unique_ptr<BaseEventTarget> target) {
    targets[name] = std::move(target);
}

void EventTargetManager::remove(const std::string &name) {
    targets.erase(name);
}
