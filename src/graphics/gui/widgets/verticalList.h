//
// Created by felix on 8/14/24.
//

#ifndef BUILDIT_VERTICALLIST_H
#define BUILDIT_VERTICALLIST_H


#include "container.h"

namespace GUI {
    class VerticalList : public Container {
    public:
        VerticalList(View* view, uintVec2 size) : Container(view, size) {};
    protected:
        uintVec2 calcChildPosition(std::_List_iterator<std::unique_ptr<Element>> iter) override;
    };
}


#endif //BUILDIT_VERTICALLIST_H
