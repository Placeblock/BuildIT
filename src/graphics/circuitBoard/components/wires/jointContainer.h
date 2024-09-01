//
// Created by felix on 8/28/24.
//

#ifndef BUILDIT_JOINTCONTAINER_H
#define BUILDIT_JOINTCONTAINER_H


#include <memory>
#include "wire.h"

class JointContainer {
public:
    virtual void addJoint(const std::shared_ptr<Joint> &joint) = 0;
    virtual void removeJoint(Joint *joint) = 0;
    virtual void setNetwork(Joint *joint, Network *network) = 0;
    virtual std::shared_ptr<Joint> getOwningRef(const Joint *joint) const = 0;
    virtual size_t getJointIndex(const Joint* joint) const = 0;
    [[nodiscard]] virtual std::set<const Joint*> getJoints() const = 0;
};


#endif //BUILDIT_JOINTCONTAINER_H
