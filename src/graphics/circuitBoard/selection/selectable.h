//
// Created by felix on 9/16/24.
//

#ifndef BUILDIT_SELECTABLE_H
#define BUILDIT_SELECTABLE_H

#include "graphics/circuitBoard/selection/selectable.h"
#include "graphics/circuitBoard/events/componentSelectEvent.h"
#include "graphics/circuitBoard/observer.h"
#include "graphics/circuitBoard/components/abstraction/component.h"

class Selectable : public Subject<SelectEvent>, public Subject<DeselectEvent>, virtual public Component {
private:
    bool selected = false;
public:
    virtual void select();
    virtual void deselect();

    [[nodiscard]] bool isSelected() const;
};


#endif //BUILDIT_SELECTABLE_H
