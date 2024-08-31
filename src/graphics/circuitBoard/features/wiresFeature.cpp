//
// Created by felix on 8/31/24.
//

#include "wiresFeature.h"

void WiresFeature::render() {
    this->wiresRenderer.render(this->programs->wireProgram, this->programs->vertexProgram);
}

WiresFeature::WiresFeature(Programs *programs) : Renderable(programs) {

}
