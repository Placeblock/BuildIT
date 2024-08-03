//
// Created by felix on 8/3/24.
//

#include <cmath>
#include <iostream>
#include "shapes.h"

void Shapes::generateRoundedRectangle(float width, float height, float radius, float* vertices) {
    vertices[0] = radius;
    vertices[1] = 0;
    vertices[2] = width-radius;
    vertices[3] = 0;

    vertices[14] = width;
    vertices[15] = radius;
    vertices[16] = width;
    vertices[17] = height-radius;

    vertices[28] = width-radius;
    vertices[29] = height;
    vertices[30] = radius;
    vertices[31] = height;

    vertices[42] = 0;
    vertices[43] = height-radius;
    vertices[44] = 0;
    vertices[45] = radius;

    for (int i = 1; i < 6; ++i) {
        float x = std::cos(M_PI/12*i)*radius;
        float y = std::sin(M_PI/12*i)*radius;
        vertices[2+i*2] = width-radius+y;
        vertices[3+i*2] = radius-x;

        vertices[16+i*2] = width-radius+x;
        vertices[17+i*2] = height-radius+y;

        vertices[30+i*2] = radius-y;
        vertices[31+i*2] = height-radius+x;

        vertices[44+i*2] = radius-x;
        vertices[45+i*2] = radius-y;
    }
}
