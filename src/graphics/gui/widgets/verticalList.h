//
// Created by felix on 8/14/24.
//

#ifndef BUILDIT_VERTICALLIST_H
#define BUILDIT_VERTICALLIST_H


#include "container.h"

class VerticalList : public Container {
protected:
    std::vector<glm::vec2> calcChildPositions() override;
};


#endif //BUILDIT_VERTICALLIST_H
