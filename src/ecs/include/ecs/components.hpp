//
// Created by felix on 25.03.25.
//

#ifndef MODELS_H
#define MODELS_H

#include <typeindex>
#include <vector>

#include "internal/globalentitymixin.hpp"
#include <glm/vec2.hpp>
#include <entt/entt.hpp>

namespace buildit::ecs {
typedef uint64_t global_entity_t;
/**
     * @brief The registry type to be used by BuildIT.
     */
typedef global_entity_mixin<entt::registry, global_entity_t> registry;

struct chip_component_t {
};

/**
 * @brief ECS Component for BuildIT components that can be
 * positioned on the circuit board.
 */
struct position_t {
    glm::vec2 pos;
};

/**
 * @brief ECS Component for BuildIT components that have a
 * specific size
 */
struct size_t {
    glm::vec2 size;
};

/**
 * @brief ECS Component for BuildIT components that can be
 * rotated on the circuit board.
 */
struct rotation {
    uint8_t rot;

    rotation &operator+=(const rotation &rhs);

    rotation operator+(const rotation &rhs) const;

    void apply(glm::vec2 &pos) const;
};

/**
 * @brief ECS Component for BuildIT components that should get rotated.
 * If a rotate event gets processed, the \ref rotation is not updated immediately.
 * Instead, the \ref rotate component is added to the components and the systems
 * move it afterward.
 */
struct rotate {
    rotation delta;
    bool disconnect;
};

/**
 * @brief Contains information about a specific Pin of a component on the circuit board.
 */
struct pin {
    glm::vec2 pos;
    std::type_index type;
    bool input;
    std::vector<void *> simPins;
    entt::entity joint;

    glm::vec2 get_abs(const glm::vec2 &componentPos, const rotation &rot) const;
};

/**
 * @brief ECS Component for a Wire
 */
struct wire {
    entt::entity left;
    entt::entity right;

    [[nodiscard]] entt::entity getOther(entt::entity joint) const;
};

/**
 * @brief ECS Entity for a Network
 */
struct network {
    std::vector<entt::entity> wires;
    std::vector<entt::entity> joints;
};
} // namespace buildit::ecs

#endif // MODELS_H
