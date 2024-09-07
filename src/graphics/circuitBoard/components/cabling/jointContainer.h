//
// Created by felix on 8/28/24.
//

#ifndef BUILDIT_JOINTCONTAINER_H
#define BUILDIT_JOINTCONTAINER_H


#include <memory>
#include "wire.h"
#include "jointEvents.h"

class JointContainer : public Subject<JointAddEvent>, public Subject<JointRemoveEvent> {
private:
    std::list<std::shared_ptr<Joint>> joints;
public:
    void addJoint(const std::shared_ptr<Joint> &joint);
    void removeJoint(Joint *j);

    [[nodiscard]] size_t getJointIndex(const Joint* joint) const;
    [[nodiscard]] std::shared_ptr<Joint> getOwningRef(const Joint* joint) const;
    [[nodiscard]] std::set<const Joint*> getJoints() const;
};


#endif //BUILDIT_JOINTCONTAINER_H
