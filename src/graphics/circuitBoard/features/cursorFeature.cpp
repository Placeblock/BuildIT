//
// Created by felix on 8/31/24.
//

#include "cursorFeature.h"


CursorFeature::CursorFeature(Camera *camera, MousePosAccessor *mpa, Programs *programs)
    : camera(camera), mpa(mpa), Renderable(programs) {
}

void CursorFeature::update(float timeStep) {
    this->cursor.update(this->mpa->getMousePos(), *this->camera, timeStep);
    this->cursorRenderer.update(this->cursor.pos);
}

void CursorFeature::render() {
    this->cursorRenderer.render(programs->vertexProgram);
}