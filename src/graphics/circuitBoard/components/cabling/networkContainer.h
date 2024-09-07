//
// Created by felix on 8/28/24.
//

#ifndef BUILDIT_NETWORKCONTAINER_H
#define BUILDIT_NETWORKCONTAINER_H


#include <memory>
#include "wire.h"
#include "networkEvents.h"

class NetworkContainer : public TypedSubject<NetworkAddEvent, Network>, public TypedSubject<NetworkRemoveEvent, Network> {
private:
    std::list<std::shared_ptr<Network>> networks;
public:
    void addNetwork(const std::shared_ptr<Network> &network);
    void removeNetwork(Network *j);

    [[nodiscard]] std::shared_ptr<Network> getOwningRef(const Network* network) const;
};


#endif //BUILDIT_NETWORKCONTAINER_H
