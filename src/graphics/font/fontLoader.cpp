//
// Created by felix on 8/11/24.
//

#include <iostream>
#include <utility>
#include "fontLoader.h"
#define STB_IMAGE_IMPLEMENTATION
#include "image/stb_image.h"

FontLoader::FontLoader(FontData data) {
    this->font.data = std::move(data);
}

void FontLoader::load() {
    glGenTextures(1, &this->font.texture);
    int width, height, nrChannels;
    unsigned char *bitmap = stbi_load("resources/font/font.png", &width, &height, &nrChannels, STBI_rgb_alpha);
    if (!bitmap) {
        std::cout << "FAILED TO LOAD TEXTURE\n";
    }
    glBindTexture(GL_TEXTURE_2D, this->font.texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, bitmap);
    stbi_image_free(bitmap);
}
