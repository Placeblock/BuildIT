//
// Created by felix on 11.05.25.
//

#ifndef WORLDDESERIALIZER_HPP
#define WORLDDESERIALIZER_HPP
#include <functional>
#include <string_view>
#include <vector>

template<typename T>
concept SerializableComponent = requires {
    { T::key } -> std::convertible_to<std::string_view>;
};

class BaseComponentSerializer {

};

template<typename S>
class ComponentSerializer {
    S& s;
};

class WorldSerializer {
    flecs::world &world;
    std::vector<std::function<void(WorldSerializer, auto)>> components;

public:
    explicit WorldSerializer(flecs::world &world);
    ~WorldSerializer() = default;

    template <typename S>
    void serialize(S& s) {

    }

    template<typename T>
    void component() {

    }

    template <typename S, SerializableComponent T>
    void serialize(S& s) {
        s.text1b(T::key);
        world.query<GlobalEntityId, T>()
                .each([&](GlobalEntityId id, T& t) {
            s.value8b(id);
            s.object(t);
        });
    }
};

class WorldDeserializer {

};



#endif //WORLDDESERIALIZER_HPP
