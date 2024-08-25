//
// Created by felix on 8/19/24.
//

#ifndef BUILDIT_UTIL_H
#define BUILDIT_UTIL_H

#include <string>
#include <GL/glew.h>
#include <stdexcept>
#include "image/stb_image.h"

namespace Util {
    uint loadTexture(const std::string& fileName) {
        int width, height, nrChannels;
        stbi_set_flip_vertically_on_load(1);
        unsigned char* bitmap = stbi_load(("resources/icons/" + fileName).c_str(), &width, &height, &nrChannels, STBI_rgb_alpha);
        if (bitmap == nullptr) {
            throw std::runtime_error("Failed to load file " + fileName);
        }
        GLuint texture = 0;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, bitmap);
        stbi_image_free(bitmap);
        return texture;
    }
}

#endif //BUILDIT_UTIL_H
