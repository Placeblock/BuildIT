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
    std::set<std::shared_ptr<Vertex>> vertices;
public:
    explicit NetworkResolver(std::set<std::shared_ptr<Vertex>> vertices);
    std::vector<std::set<std::shared_ptr<Vertex>>> resolved;
    void resolve();
};


#endif //BUILDIT_NETWORKRESOLVER_H
