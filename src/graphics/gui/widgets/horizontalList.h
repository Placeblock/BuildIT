//
// Created by felix on 8/14/24.
//

#ifndef BUILDIT_HORIZONTALLIST_H
#define BUILDIT_HORIZONTALLIST_H


#include "graphics/gui/widget.h"
#include "container.h"

class HorizontalList : public Container {
public:
    HorizontalList(GUI* gui, uintVec2 size) : Container(gui, size) {};
protected:
    std::vector<uintVec2> calcChildPositions() override;
};


#endif //BUILDIT_HORIZONTALLIST_H
