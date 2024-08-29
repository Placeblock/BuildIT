//
// Created by felix on 8/19/24.
//

#ifndef BUILDIT_UTIL_H
#define BUILDIT_UTIL_H

#include <string>
#include <random>
#include <GL/glew.h>
#include <stdexcept>
#include "image/stb_image.h"
#include "types.h"

namespace Util {
    std::random_device rd;
    std::mt19937 gen(rd());

    template<typename T>
    T random(T min, T max) {
        return std::uniform_real_distribution<T>(min, max)(gen);
    }

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

    Color hsv2rgb(glm::vec3& hsv) {
        float r, g, b;

        int i = floor(hsv.x * 6);
        float f = hsv.x * 6 - float(i);
        float p = hsv.z * (1 - hsv.y);
        float q = hsv.z * (1 - f * hsv.y);
        float t = hsv.z * (1 - (1 - f) * hsv.y);

        switch (i % 6) {
            case 0: r = hsv.z, g = t, b = p; break;
            case 1: r = q, g = hsv.z, b = p; break;
            case 2: r = p, g = hsv.z, b = t; break;
            case 3: r = p, g = q, b = hsv.z; break;
            case 4: r = t, g = p, b = hsv.z; break;
            case 5: r = hsv.z, g = p, b = q; break;
        }

        return Color{r*255, g*255, b*255};
    }
}

#endif //BUILDIT_UTIL_H
