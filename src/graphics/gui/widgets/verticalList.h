//
// Created by felix on 8/14/24.
//

#ifndef BUILDIT_VERTICALLIST_H
#define BUILDIT_VERTICALLIST_H


#include "container.h"

namespace GUI {
    class VerticalList : public Container {
    public:
        VerticalList(View* gui, uintVec2 size, Element* parent = nullptr) : Container(gui, size, parent) {};
    protected:
        std::vector<uintVec2> calcChildPositions() override;
    };
}


#endif //BUILDIT_VERTICALLIST_H
