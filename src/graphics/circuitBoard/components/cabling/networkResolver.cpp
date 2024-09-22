//
// Created by felix on 8/3/24.
//

#include "networkResolver.h"

NetworkResolver::NetworkResolver(const std::set<Joint*>& joints) {
    this->joints = joints;
}

void NetworkResolver::resolve() {
    std::set<Joint*> processingQueue;
    for (const auto &vertex: this->joints) {
        processingQueue.insert(vertex);
    }
    while (!processingQueue.empty()) {
        const auto joint = *processingQueue.begin();
        processingQueue.erase(joint);
        std::set<Joint*> networkJoints;
        std::queue<Joint*> networkQueue;

        networkQueue.push(joint);
        while (!networkQueue.empty()) {
            const auto networkJoint = networkQueue.front();
            networkQueue.pop();
            processingQueue.erase(networkJoint);
            networkJoints.insert(networkJoint);
            for (const auto &jointWires: networkJoint->wires) {
                const auto otherJoint = jointWires->getOther(networkJoint);
                if (networkJoints.contains(otherJoint)) continue;
                networkQueue.push(otherJoint);
            }
        }
        this->resolved.push_back(networkJoints);
    }
}
