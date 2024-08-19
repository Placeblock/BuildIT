//
// Created by felix on 8/11/24.
//

#include "nodeList.h"
#include "image/stb_image.h"

NodeList::NodeList(std::vector<NodeElement> elements) : elements(elements) {
    GLuint tTextures[elements.size()];
    glGenTextures(elements.size(), tTextures);
    this->iconTextures.insert(this->iconTextures.end(), &tTextures[0], &tTextures[elements.size()]);
    for (int i = 0; i < elements.size(); ++i) {
        int width, height, nrChannels;
        unsigned char *bitmap = stbi_load(("resources/icons/"+elements[i].icon).c_str(), &width, &height, &nrChannels, STBI_rgb_alpha);
        glBindTexture(GL_TEXTURE_2D, this->iconTextures[i]);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, bitmap);
        stbi_image_free(bitmap);
    }
}

void NodeList::render(Program *program) {
    program->use();
    glBindVertexArray(this->iconVAO);
    for (int i = 0; i < this->elements.size(); ++i) {
        glBindTexture(GL_TEXTURE_2D, this->iconTextures[i]);
        glDrawArrays(GL_TRIANGLES, i*6, 6);
    }
}


