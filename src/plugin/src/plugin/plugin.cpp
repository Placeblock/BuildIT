//
// Created by felix on 25.05.25.
//

#include "plugin/plugin.hpp"

#include <utility>
plugin::plugin(std::string name) : name(std::move(name)) {}