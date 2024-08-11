//
// Created by felix on 8/7/24.
//

#ifndef BUILDIT_TYPES_H
#define BUILDIT_TYPES_H

#include <vector>
#include "glm/detail/type_vec2.hpp"

typedef glm::tvec2<int, glm::highp> intVec2;
typedef glm::tvec3<unsigned char, glm::lowp> Color;

static const std::vector<float> FULL_TEXTURE_COORDS = {
        0, 1, 1, 1, 1, 0,
        1, 0, 0, 0, 0, 1
};

static const std::vector<float> I_FULL_TEXTURE_COORDS = {
        0, 0, 1, 0, 1, 1,
        1, 1, 0, 1, 0, 0
};

static const std::vector<unsigned char> FULL_TEXTURE_COLORS = {
        255, 255, 255, 255, 255, 255, 255, 255, 255,
        255, 255, 255, 255, 255, 255, 255, 255, 255
};

#endif //BUILDIT_TYPES_H
