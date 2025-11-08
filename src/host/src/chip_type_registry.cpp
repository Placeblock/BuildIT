//
// Created by felix on 18.07.25.
//

#include "host/chip_type_registry.hpp"

#include "spdlog/spdlog.h"
#include <stdexcept>

using namespace buildit::ecs;

base_chip_type_t::base_chip_type_t(const std::string &key,
                                   const uint8_t width,
                                   const uint8_t height)
    : key(key), width(width), height(height),
      simulation_storage((key + SIMULATION_STORAGE_SUFFIX).c_str(),
                         key.size() + SIMULATION_STORAGE_SUFFIX.size()),
      graphics_storage((key + GRAPHICS_STORAGE_SUFFIX).c_str(),
                       key.size() + GRAPHICS_STORAGE_SUFFIX.size()) {
}

void chip_type_registry_t::register_chip_type(const std::string &key,
                                              std::unique_ptr<base_chip_type_t> &chip_type) {
    if (this->chip_types.contains(key)) {
        throw std::runtime_error("component '" + key + "' already registered");
    }
    this->chip_types[key] = std::move(chip_type);
    spdlog::info("Registered chip type '{}'", key);
}

const std::unordered_map<std::string, std::unique_ptr<base_chip_type_t> > &
chip_type_registry_t::get_chip_types() const {
    return this->chip_types;
}

const base_chip_type_t *chip_type_registry_t::get_chip_type(const std::string &key) const {
    if (!this->chip_types.contains(key)) {
        return nullptr;
    }
    return this->chip_types.at(key).get();
}