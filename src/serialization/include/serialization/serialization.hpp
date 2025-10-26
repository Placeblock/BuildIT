//
// Created by felix on 24.05.25.
//

#ifndef SERIALIZATION_HPP
#define SERIALIZATION_HPP

#include <filesystem>

#include "../../../ecs/include/ecs/internal/snapshot.hpp"
#include "ecs/components.hpp"

namespace buildit::serialization {
inline ecs::loader_registry loader_registry;

std::unique_ptr<ecs::registry> load_registry(std::istream& stream);
std::unique_ptr<ecs::registry> load_registry(const std::filesystem::path& path);
}
    
#endif //SERIALIZATION_HPP
