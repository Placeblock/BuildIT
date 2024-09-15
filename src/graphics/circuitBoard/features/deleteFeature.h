//
// Created by felix on 9/7/24.
//

#ifndef BUILDIT_DELETEFEATURE_H
#define BUILDIT_DELETEFEATURE_H


#include "feature.h"
#include "graphics/circuitBoard/components/componentContainer.h"
#include "selectionFeature.h"
#include "graphics/circuitBoard/history/history.h"

class DeleteFeature : public Feature {
private:
    SelectionAccessor *selectionAccessor;
    History *history;
    ComponentContainer *compContainer;
public:
    explicit DeleteFeature(SelectionAccessor *selectionAccessor, History *history, ComponentContainer *compContainer)
        : compContainer(compContainer), history(history), selectionAccessor(selectionAccessor) {};

    void onKeyAction(glm::vec2 relPos, int key, int scanCode, int action, int mods) override;
};


#endif //BUILDIT_DELETEFEATURE_H
