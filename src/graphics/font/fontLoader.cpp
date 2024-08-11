//
// Created by felix on 8/11/24.
//

#include "fontLoader.h"
#define STB_IMAGE_IMPLEMENTATION
#include "image/stb_image.h"

FontLoader::FontLoader(FontData data) : data(std::move(data)) {
    this->textures.reserve(data.pageCount);
}

void FontLoader::load() {
    glGenTextures(int(this->data.pageCount), this->textures.data());
    for (int i = 0; i < this->data.pageCount; ++i) {
        Page page = this->data.pages[i];
        int width, height, nrChannels;
        unsigned char *bitmap = stbi_load(("resources/font/"+page.fileName).c_str(), &width, &height, &nrChannels, 0);

        glBindTexture(GL_TEXTURE_2D, this->textures[i]);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, bitmap);

        stbi_image_free(bitmap);
    }
}
