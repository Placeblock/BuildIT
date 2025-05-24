//
// Created by felix on 24.05.25.
//

#include "serialization/serialization.hpp"

#include <fstream>

#include "model/snapshot.hpp"

namespace buildit::serialization {
std::unique_ptr<ecs::registry> load_registry(std::istream &stream) {
    ecs::deserializer deser{stream};

    auto load_reg = std::make_unique<ecs::registry>();
    const ecs::registry_deserializer reg_deser{load_reg.get(), &loader_registry};
    reg_deser.deserialize(deser);

    return load_reg;
}

std::unique_ptr<ecs::registry> load_registry(const std::filesystem::path &path) {
    std::ifstream stream(path, std::ios::binary);
    if (!stream.is_open()) {
        spdlog::error("Could not open registry file {}", path);
        throw std::runtime_error{"Could not open file"};
    }
    return load_registry(stream);
}
}