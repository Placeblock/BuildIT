//
// Created by felix on 9/7/24.
//

#ifndef BUILDIT_SELECTIONFEATURE_H
#define BUILDIT_SELECTIONFEATURE_H


#include "feature.h"
#include "graphics/circuitBoard/data/selection.h"

class HistoryChangeEvent;

class SelectionFeature : public Feature, public SelectionAccessor, public Observer<HistoryChangeEvent> {

private:
    Selection selection;
public:
    std::list<Component*>* getComponents() override;
    void clearSelection() override;
    void addComponent(Component *component);
    void removeComponent(Component *component);

    void notify(const HistoryChangeEvent& data) override;
};


#endif //BUILDIT_SELECTIONFEATURE_H
