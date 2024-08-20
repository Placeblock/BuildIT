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
    std::set<Vertex*> vertices;
public:
    explicit NetworkResolver(std::set<Vertex*> vertices);
    std::vector<std::set<Vertex*>> resolved;
    void resolve();
};


#endif //BUILDIT_NETWORKRESOLVER_H
