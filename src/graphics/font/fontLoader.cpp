//
// Created by felix on 8/11/24.
//

#include <iostream>
#include "fontLoader.h"
#define STB_IMAGE_IMPLEMENTATION
#define STBI_FAILURE_USERMSG
#include "image/stb_image.h"

FontLoader::FontLoader(FontData data) : data(std::move(data)) {
    this->textures.reserve(data.pageCount);
}

void FontLoader::load() {
    GLuint tTextures[this->data.pageCount];
    glGenTextures(int(this->data.pageCount), tTextures);
    this->textures.insert(this->textures.begin(), &tTextures[0], &tTextures[this->data.pageCount]);
    for (int i = 0; i < this->data.pageCount; ++i) {
        Page page = this->data.pages[i];
        int width, height, nrChannels;
        unsigned char *bitmap = stbi_load(("resources/font/"+page.fileName).c_str(), &width, &height, &nrChannels, STBI_rgb_alpha);
        if (!bitmap) {
            std::cout << "FAILED TO LOAD TEXTURE\n";
        }
        glBindTexture(GL_TEXTURE_2D, this->textures[i]);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, bitmap);
        stbi_image_free(bitmap);
    }
}
