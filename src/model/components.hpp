//
// Created by felix on 25.03.25.
//

#ifndef MODELS_H
#define MODELS_H

#include <cstdint>

namespace Models {
    struct Position {
        int x;
        int y;

        template <class Archive>
        void serialize(Archive &archive) {
            archive(CEREAL_NVP(x), CEREAL_NVP(y));
        }
    };

    struct Rotation {
        uint8_t rot;

        template <class Archive>
        void serialize(Archive &archive) {
            archive(CEREAL_NVP(rot));
        }
    };
}

#endif // MODELS_H
