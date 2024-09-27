//
// Created by felix on 9/27/24.
//

#ifndef BUILDIT_COMPONENTEVENTS_H
#define BUILDIT_COMPONENTEVENTS_H

class Component;

struct ComponentAddEvent {
    Component *component;
};

struct ComponentRemoveEvent {
    Component *component;
};

#endif //BUILDIT_COMPONENTEVENTS_H
