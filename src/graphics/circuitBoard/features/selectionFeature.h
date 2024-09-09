//
// Created by felix on 9/7/24.
//

#ifndef BUILDIT_SELECTIONFEATURE_H
#define BUILDIT_SELECTIONFEATURE_H


#include "feature.h"
#include "graphics/circuitBoard/data/selection.h"

class SelectionFeature : public Feature {

private:
    Selection selection;
public:
    std::list<Component*>* getComponents();
    void clearSelection();
    void addComponent(Component *component);
    void removeComponent(Component *component);
};


#endif //BUILDIT_SELECTIONFEATURE_H
