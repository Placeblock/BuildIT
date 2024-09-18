//
// Created by felix on 9/16/24.
//

#ifndef BUILDIT_SELECTABLE_H
#define BUILDIT_SELECTABLE_H

#include "graphics/circuitBoard/selection/selectable.h"
#include "componentSelectEvent.h"
#include "graphics/circuitBoard/observer.h"
#include "graphics/circuitBoard/components/abstraction/component.h"

class Selectable : public Subject<SelectEvent>, public Subject<DeselectEvent>, virtual public Component {
public:
    virtual void select();
    virtual void deselect();
};


#endif //BUILDIT_SELECTABLE_H
