//
// Created by felix on 9/10/24.
//

#ifndef BUILDIT_HISTORYFEATURE_H
#define BUILDIT_HISTORYFEATURE_H


#include "feature.h"
#include "graphics/circuitBoard/history/history.h"
#include "graphics/circuitBoard/observer.h"

class HistoryFeature : public Feature {
private:
    History *history;
public:
    explicit HistoryFeature(History *history);
    void onKeyAction(glm::vec2 relPos, int key, int scanCode, int action, int mods) override;
};


#endif //BUILDIT_HISTORYFEATURE_H
