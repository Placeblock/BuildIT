//
// Created by felix on 8/31/24.
//

#include "wiresFeature.h"

void WiresFeature::render() {
    this->cablingRenderer.render(this->programs->wireProgram, this->programs->vertexProgram);
}

WiresFeature::WiresFeature(Programs *programs) : Renderable(programs),
    cabling(&this->joints, &this->joints, &this->wires, &this->wires),
    cablingRenderer(&this->joints, &this->joints, &this->wires, &this->wires, &this->networks, &this->networks){

}
