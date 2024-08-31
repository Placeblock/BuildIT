//
// Created by felix on 8/7/24.
//

#ifndef BUILDIT_TYPES_H
#define BUILDIT_TYPES_H

#include <vector>
#include "glm/detail/type_vec2.hpp"
#include "glm/detail/type_vec3.hpp"

typedef glm::tvec2<int, glm::highp> intVec2;
typedef glm::tvec2<unsigned int, glm::highp> uintVec2;
typedef glm::tvec3<unsigned char, glm::lowp> Color;

static const std::vector<glm::vec2> TEXTURE_COORDS_VEC = {
        {0, 1}, {1, 1}, {1, 0}, {1, 0}, {0, 0}, {0, 1}
};

static const std::vector<Color> TEXTURE_COLORS_VEC = std::vector<Color>(6, Color{255, 255, 255});

#endif //BUILDIT_TYPES_H
