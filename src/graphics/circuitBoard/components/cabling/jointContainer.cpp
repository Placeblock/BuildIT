//
// Created by felix on 9/7/24.
//

#include "jointContainer.h"

void JointContainer::addJoint(const std::shared_ptr<Joint>& joint) {
    this->joints.push_back(joint);
    this->Subject<JointAddEvent>::notify({joint.get()});
}

void JointContainer::removeJoint(Joint *joint) {
    const auto iter = std::find_if(this->joints.begin(), this->joints.end(), [&joint](const std::shared_ptr<Joint>& j){
        return j.get() == joint;
    });
    assert(iter != this->joints.end() && "Tried to remove non existent joint from joints");
    this->joints.erase(iter);
    this->Subject<JointRemoveEvent>::notify({joint});
}

size_t JointContainer::getJointIndex(const Joint* joint) const {
    const auto iter = std::find_if(this->joints.begin(), this->joints.end(), [&joint](const std::shared_ptr<Joint>& j){
        return j.get() == joint;
    });
    return std::distance(this->joints.begin(), iter);
}

std::shared_ptr<Joint> JointContainer::getOwningRef(const Joint *joint) const {
    const auto iter = std::find_if(this->joints.begin(), this->joints.end(), [&joint](const std::shared_ptr<Joint>& j){
        return j.get() == joint;
    });
    if (iter == this->joints.end()) return nullptr;
    return *iter;
}

std::set<const Joint *> JointContainer::getJoints() const {
    std::set<const Joint*> nOJoints;
    std::transform(this->joints.begin(), this->joints.end(), std::inserter(nOJoints, nOJoints.end()), [](const auto& j) {
        return j.get();
    });
    return nOJoints;
}