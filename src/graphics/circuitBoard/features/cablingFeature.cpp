//
// Created by felix on 8/31/24.
//

#include "cablingFeature.h"

void CablingFeature::render() {
    this->cablingRenderer.render(this->programs->wireProgram, this->programs->vertexProgram);
}

CablingFeature::CablingFeature(Programs *programs) : Renderable(programs),
                                                     cabling(&this->joints, &this->joints, &this->wires, &this->wires),
                                                     cablingRenderer(&this->joints, &this->joints, &this->wires, &this->wires, &this->networks, &this->networks){

}
