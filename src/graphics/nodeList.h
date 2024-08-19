//
// Created by felix on 8/11/24.
//

#ifndef BUILDIT_NODELIST_H
#define BUILDIT_NODELIST_H

#include "graphics/font/fontRenderer.h"

struct NodeElement {
    std::string name;
    std::string icon;
};

class NodeList {
public:
    NodeList(std::vector<NodeElement> elements);
    void render(Program* program);
private:
    std::vector<NodeElement> elements;
    std::vector<GLuint> iconTextures;

    std::vector<float> generateVertexData();
    std::vector<float> generateTextureData();
    std::vector<unsigned char> generateColorData();
};


#endif //BUILDIT_NODELIST_H
