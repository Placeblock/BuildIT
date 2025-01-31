//
// Created by felix on 8/31/24.
//

#include "cursorFeature.h"


CursorFeature::CursorFeature(Programs *programs, Camera *camera, MousePosAccessor *mpa)
    : Renderable(programs), camera(camera), mpa(mpa) {
}

void CursorFeature::update(const float timeStep) {
    const glm::vec2 oldPos = this->cursor.pos;
    this->cursor.update(this->mpa->getMousePos(), *this->camera, timeStep);
    if (oldPos != this->cursor.pos) {
        const glm::vec2 delta = this->cursor.pos - oldPos;
        this->notify({delta, this->cursor.pos});
        this->cursorRenderer.update(this->cursor.pos);
    }
}

void CursorFeature::render() {
    this->cursorRenderer.render(programs->vertexProgram);
}

intVec2 CursorFeature::getHoveringCell() const {
    return this->cursor.hoveringCell;
}

glm::vec2 CursorFeature::getHoveringCellDelta() const {
    return this->cursor.pos - glm::vec2(this->cursor.hoveringCell * 32);
}

glm::vec2 CursorFeature::getCursorPos() const {
    return this->cursor.pos;
}
