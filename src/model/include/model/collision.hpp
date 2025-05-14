//
// Created by felix on 09.05.25.
//

#ifndef COLLISION_HPP
#define COLLISION_HPP

#include <entt/entt.hpp>

#include "components.hpp"

namespace Model
{
class Collision
{
    const BuildIT::Registry& registry;
public:
    explicit Collision(BuildIT::Registry& registry);
    ~Collision();

    [[nodiscard]] BuildIT::Entity getEntityBB(const Model::Position& position) const;
};
}

#endif //COLLISION_HPP
