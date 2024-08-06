//
// Created by felix on 8/3/24.
//

#ifndef BUILDIT_SHAPES_H
#define BUILDIT_SHAPES_H


#include "glm/vec3.hpp"
#include "glm/detail/type_vec2.hpp"

class Shapes {
public:
    static std::vector<float> generateRoundedRectangle(int width, int height, int radius, glm::vec2 offset = glm::vec2(0, 0));
    static void getRoundedRectangleIndices(std::vector<unsigned int>* indices, int offset = 0);
    static std::vector<unsigned char> getRepeatedColor(glm::vec3 color, int count);
};


#endif //BUILDIT_SHAPES_H
