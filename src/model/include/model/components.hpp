//
// Created by felix on 25.03.25.
//

#ifndef MODELS_H
#define MODELS_H

#include "common/vector.hpp"
#include <typeindex>
#include <vector>

#include "entt/entity/registry.hpp"

#include "globalentitymixin.hpp"

namespace buildit::ecs {
/**
     * @brief The entity type which is used in the EnTT \ref registry.
     * We use a 64-bit unsigned integer, because by default
     * EnTT uses 32-bit uint which only uses 8 bit for the entity version.
     * Because I want to make sure that even if an entity identifier is used
     * more than 255 times, no reuse will happen.
     */
typedef uint64_t entity;
typedef uint64_t global_entity;
/**
     * @brief The registry type to be used by BuildIT.
     */
typedef global_entity_mixin<entt::basic_registry<entity>, global_entity> registry;

/**
 * @brief ECS Component for BuildIT components that can be
 * positioned on the circuit board.
 */
struct position {
    int x, y;

    position& operator+=(const position& other);

    position& operator*=(const int scalar);

    friend position operator+(position lhs, const position& rhs) {
        lhs += rhs;
        return lhs;
    }

    friend position operator*(position lhs, const int rhs) {
        lhs *= rhs;
        return lhs;
    }

    bool operator==(const position& other) const;
};

/**
 * @brief ECS Component for BuildIT components that have a
 * specific size
 */
struct size {
    int x, y;
};

struct move {
    Math::Vector<int> delta;
    bool disconnect;
};

/**
 * @brief ECS Component for BuildIT components that can be
 * rotated on the circuit board.
 */
struct rotation {
    uint8_t rot;

    rotation& operator+=(const rotation& rhs);
    rotation operator+(const rotation& rhs) const;

    void apply(position& pos) const;
};

/**
 * @brief ECS Component for BuildIT components that should get rotated.
 * If a rotate event gets processed, the \ref rotation is not updated immediately.
 * Instead, the \ref rotate component is added to the components and the systems
 * move it afterwards.
 */
struct rotate {
    rotation delta;
    bool disconnect;
};

/**
 * @brief Contains information about a specific Pin of a component on the circuit board.
 */
struct pin {
    position pos;
    std::type_index type;
    bool input;
    std::vector<void*> simPins;
    entity joint;

    position get_abs(const position& componentPos, const rotation& rot) const;
};

/**
 * @brief ECS Component for a Wire
 */
struct wire {
    entity left;
    entity right;
    [[nodiscard]] entity getOther(entity joint) const;
};
/**
 * @brief ECS Entity for a Network
 */
struct network {
    std::vector<entity> wires;
    std::vector<entity> joints;
};
} // namespace buildit::ecs

#endif // MODELS_H
