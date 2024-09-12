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
#include "graphics/circuitBoard/history/history.h"

/**
 * Adds functionality for rendering wires and joints on the circuit board.
 */
class CablingFeature : public Feature, public Renderable,
        public Subject<NetworksSplitEvent>, public Subject<NetworksMergeEvent>,
        public Observer<ComponentRemoveEvent>,
        public Observer<WireAddEvent>, public Observer<WireRemoveEvent> {
private:
    History *history;
    CablingRenderer cablingRenderer;
public:
    WireContainer wires;
    NetworkContainer networks;
    Cabling cabling;

    CablingFeature(Programs *programs, History *history,
                   Subject<ComponentAddEvent> *addSubject,
                   Subject<ComponentRemoveEvent> *removeSubject);

    void notify(const ComponentRemoveEvent& data) override;
    void notify(const WireAddEvent& data) override;
    void notify(const WireRemoveEvent& data) override;

    void render() override;
};


#endif //BUILDIT_CABLINGFEATURE_H
