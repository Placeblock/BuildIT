//
// Created by felix on 8/14/24.
//

#ifndef BUILDIT_VERTICALLIST_H
#define BUILDIT_VERTICALLIST_H


#include "container.h"

class VerticalList : public Container {
public:
    VerticalList(GUI* gui, uintVec2 size) : Container(gui, size) {};
protected:
    std::vector<uintVec2> calcChildPositions() override;
};


#endif //BUILDIT_VERTICALLIST_H
