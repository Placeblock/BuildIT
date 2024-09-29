//
// Created by felix on 9/1/24.
//

#ifndef BUILDIT_ROTATABLE_H
#define BUILDIT_ROTATABLE_H

#include "graphics/circuitBoard/observer.h"

#include "component.h"
#include "graphics/circuitBoard/events/rotateEvent.h"

class Rotatable;

/**
 * Marks a component as rotatable
 */
class Rotatable : public Subject<RotateEvent>, virtual public Component {
    /**
     * Rotation in radians
     */
    float rotation = 0;
public:
    Rotatable(std::string cnamespace, std::string ckey, float rotation = 0);
    explicit Rotatable(const Rotatable &other);

    [[nodiscard]] float getRotation() const;
    virtual void rotate(float newRotation);
};

#endif //BUILDIT_ROTATABLE_H
