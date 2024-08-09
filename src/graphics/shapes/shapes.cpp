//
// Created by felix on 8/3/24.
//

#include <cmath>
#include <vector>
#include <iostream>
#include "shapes.h"

std::vector<float> Shapes::generateRoundedRectangle(int width, int height, int radius, glm::vec2 offset) {
    std::vector<float> vertices = std::vector<float>(58);
    vertices[0] = width/2 + offset.x;
    vertices[1] = height/2 + offset.y;
    vertices[2] = radius + offset.x;
    vertices[3] = offset.y;
    vertices[4] = width-radius + offset.x;
    vertices[5] = offset.y;

    vertices[16] = width + offset.x;
    vertices[17] = radius + offset.y;
    vertices[18] = width + offset.x;
    vertices[19] = height-radius + offset.y;

    vertices[30] = width-radius + offset.x;
    vertices[31] = height + offset.y;
    vertices[32] = radius + offset.x;
    vertices[33] = height + offset.y;

    vertices[44] = offset.x;
    vertices[45] = height-radius + offset.y;
    vertices[46] = offset.x;
    vertices[47] = radius + offset.y;

    for (int i = 1; i < 6; ++i) {
        float x = std::cos(M_PI/12*i)*radius;
        float y = std::sin(M_PI/12*i)*radius;
        vertices[4+i*2] = width-radius+y + offset.x;
        vertices[5+i*2] = radius-x + offset.y;

        vertices[18+i*2] = width-radius+x + offset.x;
        vertices[19+i*2] = height-radius+y + offset.y;

        vertices[32+i*2] = radius-y + offset.x;
        vertices[33+i*2] = height-radius+x + offset.y;

        vertices[46+i*2] = radius-x + offset.x;
        vertices[47+i*2] = radius-y + offset.y;
    }
    return vertices;
}

void Shapes::getRoundedRectangleIndices(std::vector<unsigned int>* indices, int offset) {
    indices->reserve(indices->size()+84);
    for (int i = 1; i < 28; i++) {
        indices->push_back(offset);
        indices->push_back(i + offset);
        indices->push_back(i+1 + offset);
    }
    indices->push_back(offset);
    indices->push_back(28 + offset);
    indices->push_back(1 + offset);
}

std::vector<unsigned char> Shapes::getRepeatedColor(glm::vec3 color, int count) {
    std::vector<unsigned char> data;
    data.resize(count*3);
    for (int i = 0; i < count; i++) {
        data[i*3] = color.x;
        data[i*3+1] = color.y;
        data[i*3+2] = color.z;
    }
    return data;
}
