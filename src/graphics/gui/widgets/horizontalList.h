//
// Created by felix on 8/14/24.
//

#ifndef BUILDIT_HORIZONTALLIST_H
#define BUILDIT_HORIZONTALLIST_H


#include "graphics/gui/widget.h"
#include "container.h"

class HorizontalList : public Container {
protected:
    std::vector<glm::vec2> calcChildPositions() override;
};


#endif //BUILDIT_HORIZONTALLIST_H
