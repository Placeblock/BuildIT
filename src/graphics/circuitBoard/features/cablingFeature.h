//
// Created by felix on 8/31/24.
//

#ifndef BUILDIT_CABLINGFEATURE_H
#define BUILDIT_CABLINGFEATURE_H

#include "graphics/circuitBoard/components/cabling/cabling.h"
#include "graphics/circuitBoard/renderer/cablingRenderer.h"
#include "feature.h"
#include "graphics/circuitBoard/components/componentContainer.h"

/**
 * Adds functionality for rendering wires and joints on the circuit board.
 */
class CablingFeature : public Feature, public Renderable,
        public Observer<ComponentAddEvent>, public Observer<ComponentRemoveEvent>,
        public Observer<WireAddEvent>, public Observer<WireRemoveEvent>,
        public Subject<NetworksSplitEvent>, public Subject<NetworksMergeEvent> {
private:
    CablingRenderer cablingRenderer;
public:
    explicit CablingFeature(Programs *programs);

    WireContainer wires;
    NetworkContainer networks;
    Cabling cabling;

    void notify(Subject<ComponentAddEvent> *subject, const ComponentAddEvent& data) override;
    void notify(Subject<ComponentRemoveEvent> *subject, const ComponentRemoveEvent& data) override;
    void notify(Subject<WireAddEvent> *subject, const WireAddEvent& data) override;
    void notify(Subject<WireRemoveEvent> *subject, const WireRemoveEvent& data) override;

    void render() override;
};


#endif //BUILDIT_CABLINGFEATURE_H
