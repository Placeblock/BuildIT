//
// Created by felix on 8/3/24.
//

#ifndef BUILDIT_NETWORKRESOLVER_H
#define BUILDIT_NETWORKRESOLVER_H


#include <queue>
#include <memory>
#include <set>
#include "wires.h"

// Goes through all elements and checks if they are connected.
class NetworkResolver {
private:
    std::set<Joint*> joints;
public:
    explicit NetworkResolver(const std::set<Joint*>& joints);
    std::vector<std::set<Joint*>> resolved;
    void resolve();
};


#endif //BUILDIT_NETWORKRESOLVER_H
