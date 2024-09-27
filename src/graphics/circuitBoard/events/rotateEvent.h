//
// Created by felix on 9/27/24.
//

#ifndef BUILDIT_ROTATEEVENT_H
#define BUILDIT_ROTATEEVENT_H

class Rotatable;

struct RotateEvent {
    Rotatable *rotatable;
    float newRotation;
    float delta;
    bool before;
};

#endif //BUILDIT_ROTATEEVENT_H
