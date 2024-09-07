//
// Created by felix on 8/19/24.
//

#ifndef BUILDIT_UTIL_H
#define BUILDIT_UTIL_H

#include <string>
#include <random>
#include <GL/glew.h>
#include <stdexcept>
#include <iostream>
#include "image/stb_image.h"
#include "glm/vec3.hpp"
#include "types.h"
#include "graphics/buffer/vertexBuffer.h"

namespace Util {
    float random(float min = 0, float max = 1);

    unsigned int loadTexture(const std::string& fileName);

    Color hsv2rgb(glm::vec3 hsv);

    BufferLayout getDefaultVertexLayout();
    BufferLayout getDefaultColorLayout();
    BufferLayout getDefaultLayout();
}

#endif //BUILDIT_UTIL_H
