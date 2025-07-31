//
// Created by felix on 17.07.25.
//

#ifndef PLAYER_H
#define PLAYER_H
#include <cstdint>
#include <string>

namespace buildit {
typedef uint32_t player_id;
struct player {
    uint32_t id;
    std::string name;
};
} // namespace buildit

template<>
struct std::hash<buildit::player> {
    std::size_t operator()(const buildit::player& k) const noexcept {
        return std::hash<int>()(k.id);
    }
};

#endif //PLAYER_H
