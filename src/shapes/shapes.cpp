//
// Created by felix on 8/3/24.
//

#include <cmath>
#include <vector>
#include <iostream>
#include "shapes.h"

std::vector<float> Shapes::generateRoundedRectangle(int width, int height, int radius) {
    std::vector<float> vertices = std::vector<float>(58);
    vertices[0] = width/2;
    vertices[1] = height/2;
    vertices[2] = radius;
    vertices[3] = 0;
    vertices[4] = width-radius;
    vertices[5] = 0;

    vertices[16] = width;
    vertices[17] = radius;
    vertices[18] = width;
    vertices[19] = height-radius;

    vertices[30] = width-radius;
    vertices[31] = height;
    vertices[32] = radius;
    vertices[33] = height;

    vertices[44] = 0;
    vertices[45] = height-radius;
    vertices[46] = 0;
    vertices[47] = radius;

    for (int i = 1; i < 6; ++i) {
        float x = std::cos(M_PI/12*i)*radius;
        float y = std::sin(M_PI/12*i)*radius;
        vertices[4+i*2] = width-radius+y;
        vertices[5+i*2] = radius-x;

        vertices[18+i*2] = width-radius+x;
        vertices[19+i*2] = height-radius+y;

        vertices[32+i*2] = radius-y;
        vertices[33+i*2] = height-radius+x;

        vertices[46+i*2] = radius-x;
        vertices[47+i*2] = radius-y;
    }
    return vertices;
}

std::vector<unsigned int> Shapes::getRoundedRectangleIndices() {
    std::vector<unsigned int> indices = std::vector<unsigned int>();
    indices.reserve(168);
    for (int i = 1; i < 28; i++) {
        indices.push_back(0);
        indices.push_back(i);
        indices.push_back(i+1);
    }
    indices.push_back(0);
    indices.push_back(28);
    indices.push_back(1);
    return indices;
}
