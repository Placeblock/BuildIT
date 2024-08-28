//
// Created by felix on 8/28/24.
//

#ifndef BUILDIT_WIRECONTAINER_H
#define BUILDIT_WIRECONTAINER_H


#include "wire.h"

class WireContainer {
public:
    virtual void addWire(const std::shared_ptr<Wire>& wire) = 0;
    virtual void removeWire(Wire *wire) = 0;
    virtual size_t getWireIndex(const Wire *wire) const = 0;
    [[nodiscard]] virtual std::set<const Wire*> getWires() const = 0;
};


#endif //BUILDIT_WIRECONTAINER_H
