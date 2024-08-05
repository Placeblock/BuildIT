//
// Created by felix on 8/3/24.
//

#ifndef BUILDIT_SHAPES_H
#define BUILDIT_SHAPES_H


class Shapes {
public:
    static std::vector<float> generateRoundedRectangle(int width, int height, int radius);
    static std::vector<unsigned int> getRoundedRectangleIndices();
};


#endif //BUILDIT_SHAPES_H
