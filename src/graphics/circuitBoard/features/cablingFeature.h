//
// Created by felix on 8/31/24.
//

#ifndef BUILDIT_CABLINGFEATURE_H
#define BUILDIT_CABLINGFEATURE_H

#include "graphics/circuitBoard/observer.h"
#include "graphics/circuitBoard/components/cabling/cabling.h"
#include "graphics/circuitBoard/components/renderer/cablingRenderer.h"
#include "feature.h"
#include "graphics/circuitBoard/components/componentContainer.h"
#include "graphics/circuitBoard/components/cabling/networkContainer.h"
#include "graphics/circuitBoard/history/history.h"

/**
 * Adds functionality for rendering wires and joints on the circuit board.
 */
class CablingFeature final : public Feature,
                             public Subject<NetworksSplitEvent>, public Subject<NetworksMergeEvent>,
                             public Observer<ComponentAddEvent>, public Observer<ComponentRemoveEvent> {
    History *history;
public:
    NetworkContainer networks;
    CablingRenderer *cablingRenderer;
    Cabling cabling;

    CablingFeature(History *history,
                   Subject<ComponentAddEvent> *addSubject,
                   Subject<ComponentRemoveEvent> *removeSubject,
                   CablingRenderer *cablingRenderer);

    void notify(const ComponentAddEvent& data) override;
    void notify(const ComponentRemoveEvent& data) override;

};


#endif //BUILDIT_CABLINGFEATURE_H
