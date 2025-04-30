//
// Created by felix on 28.04.25.
//

#ifndef MOVEEVENTHANDLER_H
#define MOVEEVENTHANDLER_H

#include "events/event.hpp"

namespace Events {
    void moveEventHandler(const MoveEvent&);
    void rotateEventHandler(const RotateEvent&);
}

#endif //MOVEEVENTHANDLER_H
