//
// Created by felix on 09.05.25.
//

#ifndef COLLISION_HPP
#define COLLISION_HPP

#include "components.hpp"

namespace buildit::ecs {
class collision {
    const registry& reg;

public:
    explicit collision(registry& reg);

    [[nodiscard]] entity getEntityBB(const position& position) const;
};
} // namespace buildit::ecs

#endif //COLLISION_HPP
