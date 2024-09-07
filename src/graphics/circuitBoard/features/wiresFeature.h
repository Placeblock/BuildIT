//
// Created by felix on 8/31/24.
//

#ifndef BUILDIT_WIRESFEATURE_H
#define BUILDIT_WIRESFEATURE_H

#include "graphics/circuitBoard/components/wires/cabling.h"
#include "graphics/circuitBoard/renderer/wiresRenderer.h"
#include "feature.h"

/**
 * Adds functionality for rendering wires and joints on the circuit board.
 */
class WiresFeature : public Feature, public Renderable {
private:
    Cabling wires{};
    CablingRenderer wiresRenderer;
public:
    explicit WiresFeature(Programs *programs);
    void render() override;
};


#endif //BUILDIT_WIRESFEATURE_H
