//
// Created by felix on 7/31/24.
//

#ifndef BUILDIT_NODE_H
#define BUILDIT_NODE_H


#include <raylib.h>

namespace Graphics {
    class Node {
    public:
        Node(int x, int y) {}

        int x{}, y{};
        bool rendered = true;
        int rotation = 0; // 1 = 90deg 2 = 180deg 3 = 270deg

        virtual void updateRendered(int lod, Vector2 offset) = 0;
        virtual void render(int lod) const = 0;
    };
}


#endif //BUILDIT_NODE_H
