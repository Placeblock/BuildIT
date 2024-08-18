//
// Created by felix on 8/14/24.
//

#ifndef BUILDIT_HORIZONTALLIST_H
#define BUILDIT_HORIZONTALLIST_H


#include "graphics/gui/widget.h"
#include "container.h"

namespace GUI {
    class HorizontalList : public Container {
    public:
        HorizontalList(View *gui, uintVec2 size, Element* parent = nullptr) : Container(gui, size, parent) {};
    protected:
        std::vector<uintVec2> calcChildPositions() override;
    };
}

#endif //BUILDIT_HORIZONTALLIST_H
