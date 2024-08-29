//
// Created by felix on 8/14/24.
//

#ifndef BUILDIT_HORIZONTALLIST_H
#define BUILDIT_HORIZONTALLIST_H

#include "container.h"
#include <list>
#include <memory>
#include "graphics/types.h"

namespace GUI {
    class Element;
    class View;

    class HorizontalList : public Container {
    public:
        HorizontalList(View *view, uintVec2 size) : Container(view, size) {};
    protected:
        uintVec2 calcChildPosition(std::_List_iterator<std::unique_ptr<Element>> iter) override;
    };
}

#endif //BUILDIT_HORIZONTALLIST_H
