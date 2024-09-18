//
// Created by felix on 9/7/24.
//

#ifndef BUILDIT_DELETEFEATURE_H
#define BUILDIT_DELETEFEATURE_H


#include "feature.h"
#include "graphics/circuitBoard/components/componentContainer.h"
#include "selectionFeature.h"
#include "graphics/circuitBoard/history/history.h"

class DeleteFeature final : public Feature {
    SelectionAccessor *selectionAccessor;
    History *history;
    ComponentContainer *compContainer;
public:
    explicit DeleteFeature(SelectionAccessor *selectionAccessor, History *history, ComponentContainer *compContainer)
        : selectionAccessor(selectionAccessor), history(history), compContainer(compContainer) {};

    void onKeyAction(glm::vec2 relPos, int key, int scanCode, int action, int mods) override;
};


#endif //BUILDIT_DELETEFEATURE_H
