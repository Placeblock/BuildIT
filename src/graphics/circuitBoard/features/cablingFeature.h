//
// Created by felix on 8/31/24.
//

#ifndef BUILDIT_CABLINGFEATURE_H
#define BUILDIT_CABLINGFEATURE_H

#include "graphics/circuitBoard/components/cabling/cabling.h"
#include "graphics/circuitBoard/renderer/cablingRenderer.h"
#include "feature.h"

/**
 * Adds functionality for rendering wires and joints on the circuit board.
 */
class CablingFeature : public Feature, public Renderable {
private:
    CablingRenderer cablingRenderer;
public:
    explicit CablingFeature(Programs *programs);

    WireContainer wires;
    JointContainer joints;
    NetworkContainer networks;
    Cabling cabling;

    void render() override;
};


#endif //BUILDIT_CABLINGFEATURE_H
