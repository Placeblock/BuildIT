//
// Created by felix on 8/28/24.
//

#ifndef BUILDIT_JOINTCONTAINER_H
#define BUILDIT_JOINTCONTAINER_H


#include <memory>
#include "wire.h"

class JointContainer {
    virtual void addJoint(const std::shared_ptr<Joint> &joint) = 0;
    virtual void removeJoint(Joint* joint) = 0;
    virtual void moveJoint(Joint* joint, glm::vec2 newPos) = 0;
};


#endif //BUILDIT_JOINTCONTAINER_H
