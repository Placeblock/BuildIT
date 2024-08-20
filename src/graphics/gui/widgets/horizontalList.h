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
        HorizontalList(View *view, uintVec2 size) : Container(view, size) {};
    protected:
        uintVec2 calcChildPosition(std::_List_iterator<std::unique_ptr<Element>> iter) override;
    };
}

#endif //BUILDIT_HORIZONTALLIST_H
