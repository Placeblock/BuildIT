//
// Created by felix on 8/28/24.
//

#ifndef BUILDIT_NETWORKCONTAINER_H
#define BUILDIT_NETWORKCONTAINER_H


#include <memory>
#include "wire.h"

class NetworkContainer {
public:
    virtual void addNetwork(const std::shared_ptr<Network>& network) = 0;
    virtual void removeNetwork(Network *network) = 0;
    virtual std::shared_ptr<Network> getOwningRef(const Network *network) const = 0;
};


#endif //BUILDIT_NETWORKCONTAINER_H
