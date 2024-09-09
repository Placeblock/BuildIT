//
// Created by felix on 8/31/24.
//

#ifndef BUILDIT_CABLINGFEATURE_H
#define BUILDIT_CABLINGFEATURE_H

#include "graphics/circuitBoard/components/cabling/cabling.h"
#include "graphics/circuitBoard/renderer/cablingRenderer.h"
#include "feature.h"
#include "graphics/circuitBoard/components/componentContainer.h"
#include "graphics/circuitBoard/history/history.h"

/**
 * Adds functionality for rendering wires and joints on the circuit board.
 */
class CablingFeature : public Feature, public Renderable,
        public Observer<ComponentAddEvent>, public Observer<ComponentRemoveEvent>,
        public Observer<WireAddEvent>, public Observer<WireRemoveEvent>,
        public Subject<NetworksSplitEvent>, public Subject<NetworksMergeEvent> {
private:
    History *history;
    WireContainer wires;
    NetworkContainer networks;
    Cabling cabling;
    CablingRenderer cablingRenderer;
public:
    CablingFeature(Programs *programs, History *history);

    void notify(const ComponentAddEvent& data) override;
    void notify(const ComponentRemoveEvent& data) override;
    void notify(const WireAddEvent& data) override;
    void notify(const WireRemoveEvent& data) override;

    void render() override;
};


#endif //BUILDIT_CABLINGFEATURE_H
