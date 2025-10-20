//
// Created by felix on 25.05.25.
//

#include "plugin/plugin.h"

#include <utility>
buildit::plugin::plugin::plugin(std::string name) : name(std::move(name)) {
}