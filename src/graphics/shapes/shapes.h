//
// Created by felix on 8/3/24.
//

#ifndef BUILDIT_SHAPES_H
#define BUILDIT_SHAPES_H

#include <vector>
#include "glm/vec2.hpp"
#include "graphics/types.h"
#include "graphics/buffer/vertexBuffer.h"

class Shapes {
public:
    static std::vector<glm::vec2> generateRoundedRectangleVertices(int width, int height, int radius, glm::vec2 offset = glm::vec2(0, 0));
    static void getRoundedRectangleIndices(std::vector<unsigned int>& indices, int offset = 0);
    static std::vector<unsigned char> getRepeatedColor(Color color, int count);
    static std::vector<VertexData> generateRoundedRectangle(int width, int height, int radius, Color color, glm::vec2 offset = glm::vec2(
            0, 0));
};


#endif //BUILDIT_SHAPES_H
