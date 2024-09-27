//
// Created by felix on 9/16/24.
//

#ifndef BUILDIT_COMPONENTSELECTEVENT_H
#define BUILDIT_COMPONENTSELECTEVENT_H

class Selectable;

struct SelectEvent {
    Selectable *selectable;
};

struct DeselectEvent {
    Selectable *selectable;
};


#endif //BUILDIT_COMPONENTSELECTEVENT_H
