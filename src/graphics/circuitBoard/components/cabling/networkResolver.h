//
// Created by felix on 8/3/24.
//

#ifndef BUILDIT_NETWORKRESOLVER_H
#define BUILDIT_NETWORKRESOLVER_H


#include <queue>
#include <memory>
#include <set>
#include "wire.h"

typedef std::vector<std::set<Joint*>> ResolvedNetwork;

/**
 * Accepts a set of joints and returns a vector of sets. Each sets is a resolved network.
 * Joints in one set are connected through arbitrary wires. Joints in two sets are never connected in some way.
 */
class NetworkResolver {
private:
    std::set<Joint*> joints;
public:
    explicit NetworkResolver(const std::set<Joint*>& joints);
    ResolvedNetwork resolved;
    void resolve();
};


#endif //BUILDIT_NETWORKRESOLVER_H
