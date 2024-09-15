//
// Created by felix on 8/3/24.
//

#include "shapes.h"
#include <cmath>
#include <vector>

std::vector<glm::vec2> Shapes::generateRoundedRectangleVertices(int width, int height, int radius, glm::vec2 offset) {
    std::vector<glm::vec2> vertices = std::vector<glm::vec2>(29);
    vertices[0] = {width/2 + offset.x, height/2 + offset.y};
    vertices[1] = {radius + offset.x, offset.y};
    vertices[2] = {width-radius + offset.x, offset.y};

    vertices[8] = {width + offset.x, radius + offset.y};
    vertices[9] = {width + offset.x, height-radius + offset.y};

    vertices[15] = {width - radius + offset.x, height + offset.y};
    vertices[16] = {radius + offset.x, height + offset.y};

    vertices[22] = {offset.x, height-radius + offset.y};
    vertices[23] = {offset.x, radius + offset.y};

    for (int i = 1; i < 6; ++i) {
        float x = std::cos(M_PI/12*i)*radius;
        float y = std::sin(M_PI/12*i)*radius;
        vertices[2+i] = {width - radius + y + offset.x, radius-x + offset.y};
        vertices[9+i] = {width - radius + x + offset.x, height-radius+y + offset.y};
        vertices[16+i] = {radius - y + offset.x, height-radius+x + offset.y};
        vertices[23+i] = {radius - x + offset.x, radius-y + offset.y};
    }
    return vertices;
}

void Shapes::getRoundedRectangleIndices(std::vector<unsigned int>& indices, int offset) {
    indices.reserve(indices.size()+84);
    for (int i = 1; i < 28; i++) {
        indices.push_back(offset);
        indices.push_back(i + offset);
        indices.push_back(i+1 + offset);
    }
    indices.push_back(offset);
    indices.push_back(28 + offset);
    indices.push_back(1 + offset);
}

std::vector<unsigned char> Shapes::getRepeatedColor(Color color, int count) {
    std::vector<unsigned char> data;
    data.resize(count*3);
    for (int i = 0; i < count; i++) {
        data[i*3] = color.x;
        data[i*3+1] = color.y;
        data[i*3+2] = color.z;
    }
    return data;
}

std::vector<VertexData> Shapes::generateRoundedRectangle(int width, int height, int radius, Color color, glm::vec2 offset) {
    std::vector<VertexData> data;
    std::vector<glm::vec2> vertices = Shapes::generateRoundedRectangleVertices(width, height, radius, offset);
    for (const auto &item: vertices) {
        data.emplace_back(item, color);
    }
    return data;
}
