//
// Created by felix on 8/28/24.
//

#ifndef BUILDIT_WIRECONTAINER_H
#define BUILDIT_WIRECONTAINER_H


#include "wire.h"
#include "wireEvents.h"

class WireContainer : public Subject<WireAddEvent>, public Subject<WireRemoveEvent> {
private:
    std::list<std::shared_ptr<Wire>> wires;
public:
    void addWire(const std::shared_ptr<Wire>& wire);
    void removeWire(Wire *w);

    [[nodiscard]] size_t getWireIndex(const Wire *w) const;
    [[nodiscard]] std::shared_ptr<Wire> getOwningRef(const Wire *w) const;
    [[nodiscard]] std::set<const Wire*> getWires() const;
};


#endif //BUILDIT_WIRECONTAINER_H
