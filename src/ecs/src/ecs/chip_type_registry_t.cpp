//
// Created by felix on 18.07.25.
//

#include "ecs/chip_type_registry_t.hpp"
#include <stdexcept>

void chip_type_registry_t::register_chip_type(const std::string &key,
                                              std::unique_ptr<base_chip_type_t> &chip_type) {
    if (this->chip_types.contains(key)) {
        throw std::runtime_error("component '" + key + "' already registered");
    }
    this->chip_types[key] = std::move(chip_type);
}

const std::unordered_map<std::string, std::unique_ptr<base_chip_type_t> > &
chip_type_registry_t::get_chip_types() const {
    return this->chip_types;
}

const base_chip_type_t *chip_type_registry_t::get_chip_type(const std::string &key) const {
    if (!this->chip_types.contains(key)) {
        throw std::runtime_error("component '" + key + "' not registered");
    }
    return this->chip_types.at(key).get();
}