//
// Created by felix on 8/11/24.
//

#include "nodeList.h"
#include "image/stb_image.h"

NodeList::NodeList(FontRenderer *fontRenderer, std::vector<NodeElement> elements) : fontRenderer(fontRenderer), elements(elements) {
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

        this->fontRenderer->addText(elements[i].name, Alignment::CENTER, glm::vec2(80, i*200+20), 16, Color{150, 150, 0});
    }
    glGenVertexArrays(1, &this->iconVAO);
    glBindVertexArray(this->iconVAO);
    glGenBuffers(3, this->iconVBOs);
    glBindBuffer(GL_ARRAY_BUFFER, this->iconVBOs[0]);
    std::vector<float> vertexData = this->generateVertexData();
    glBufferData(GL_ARRAY_BUFFER, vertexData.size()*sizeof(float), vertexData.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (void*)nullptr);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, this->iconVBOs[1]);
    std::vector<float> textureData = this->generateTextureData();
    glBufferData(GL_ARRAY_BUFFER, sizeof(float)*textureData.size(), textureData.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)nullptr);
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, this->iconVBOs[2]);
    std::vector<unsigned char> colorData = this->generateColorData();
    glBufferData(GL_ARRAY_BUFFER, colorData.size(), colorData.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(2, 3, GL_UNSIGNED_BYTE, GL_TRUE, 0, (void*)nullptr);
    glEnableVertexAttribArray(2);
}

std::vector<float> NodeList::generateVertexData() {
    std::vector<float> vertexData(this->elements.size()*12);
    for (int i = 0; i < this->elements.size(); ++i) {
        vertexData[i*12] = 20.0f;
        vertexData[i*12+1] = 60.0f+200.0f*i;
        vertexData[i*12+2] = 140.0f;
        vertexData[i*12+3] = 60.0f+200.0f*i;
        vertexData[i*12+4] = 140.0f;
        vertexData[i*12+5] = 180.0f+200.0f*i;
        vertexData[i*12+6] = 140.0f;
        vertexData[i*12+7] = 180.0f+200.0f*i;
        vertexData[i*12+8] = 20.0f;
        vertexData[i*12+9] = 180.0f+200.0f*i;
        vertexData[i*12+10] = 20.0f;
        vertexData[i*12+11] = 60.0f+200.0f*i;
    }
    return vertexData;
}

std::vector<float> NodeList::generateTextureData() {
    std::vector<float> textureData;
    textureData.reserve(this->elements.size()*12);
    for (int i = 0; i < this->elements.size(); ++i) {
        textureData.insert(textureData.end(), I_FULL_TEXTURE_COORDS.begin(), I_FULL_TEXTURE_COORDS.end());
    }
    return textureData;
}

std::vector<unsigned char> NodeList::generateColorData() {
    std::vector<unsigned char> colorData;
    colorData.reserve(this->elements.size() * 18);
    for (int i = 0; i < this->elements.size(); ++i) {
        colorData.insert(colorData.end(), FULL_TEXTURE_COLORS.begin(), FULL_TEXTURE_COLORS.end());
    }
    return colorData;
}

void NodeList::render(Program *program) {
    program->use();
    glBindVertexArray(this->iconVAO);
    for (int i = 0; i < this->elements.size(); ++i) {
        glBindTexture(GL_TEXTURE_2D, this->iconTextures[i]);
        glDrawArrays(GL_TRIANGLES, i*6, 6);
    }
}


