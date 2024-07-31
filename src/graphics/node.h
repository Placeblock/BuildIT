//
// Created by felix on 7/31/24.
//

#ifndef BUILDIT_G_NODE_H
#define BUILDIT_G_NODE_H


#include <raylib.h>

namespace Graphics {
    class Node {
    public:
        Node(Vector2 pos);

        Vector2 pos;

        virtual void render(int lod) const = 0;
    };
}


#endif //BUILDIT_G_NODE_H
