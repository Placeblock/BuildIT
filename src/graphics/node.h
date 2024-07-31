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
        bool rendered = false;

        virtual bool updateRedered(int lod, Vector2 offset) = 0;
        virtual void render(int lod) const = 0;
    };
}


#endif //BUILDIT_NODE_H
