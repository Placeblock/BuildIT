//
// Created by felix on 25.03.25.
//

#ifndef MODELS_H
#define MODELS_H

#include <typeindex>
#include <vector>
#include "common/vector.hpp"

#include "entt/entity/registry.hpp"

namespace BuildIT
{
    /**
     * @brief The entity type which is used in the EnTT \ref registry.
     * We use a 64-bit unsigned integer, because by default
     * EnTT uses 32-bit uint which only uses 8 bit for the entity version.
     * Because I want to make sure that even if an entity identifier is used
     * more than 255 times, no reuse will happen.
     */
    typedef uint64_t Entity;
    /**
     * @brief The registry type to be used by BuildIT.
     */
    typedef entt::basic_registry<Entity> Registry;
}

namespace Model
{

/**
 * @brief ECS Component for BuildIT components that can be
 * positioned on the circuit board.
 */
struct Position {
    int x, y;

    Position& operator+=(const Position& other);

    Position& operator*=(const int scalar);

    friend Position operator+(Position lhs, const Position& rhs) {
        lhs += rhs;
        return lhs;
    }

    friend Position operator*(Position lhs, const int rhs) {
        lhs *= rhs;
        return lhs;
    }

    bool operator==(const Position& other) const;
};

/**
 * @brief ECS Component for BuildIT components that have a
 * specific size
 */
struct Size {
    int x, y;
};

struct Move
{
    Math::Vector<int> delta;
    bool disconnect;
};

/**
 * @brief ECS Component for BuildIT components that can be
 * rotated on the circuit board.
 */
struct Rotation
{
    uint8_t rot;

    Rotation &operator+=(const Rotation& rhs);
    Rotation operator+(const Rotation & rhs) const;

    void apply(Position& pos) const;
};

/**
 * @brief ECS Component for BuildIT components that should get rotated.
 * If a rotate event gets processed, the \ref rotation is not updated immediately.
 * Instead, the \ref rotate component is added to the components and the systems
 * move it afterwards.
 */
struct Rotate
{
    Rotation delta;
    bool disconnect;
};

/**
 * @brief Contains information about a specific Pin of a component on the circuit board.
 */
struct Pin
{
    Position pos;
    std::type_index type;
    bool input;
    std::vector<void *> simPins;
    BuildIT::Entity joint;

    Position get_abs(const Position& componentPos, const Rotation& rot) const;
};

/**
 * @brief ECS Component for a Wire
 */
struct Wire
{
    BuildIT::Entity left;
    BuildIT::Entity right;
    [[nodiscard]] BuildIT::Entity getOther(BuildIT::Entity joint) const;
};
/**
 * @brief ECS Entity for a Network
 */
struct Network
{
    std::vector<BuildIT::Entity> wires;
    std::vector<BuildIT::Entity> joints;
};
}

#endif // MODELS_H
