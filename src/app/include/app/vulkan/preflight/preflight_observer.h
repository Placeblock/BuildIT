//
// Created by felix on 30.09.25.
//

#ifndef PREFLIGHT_OBSERVER_H
#define PREFLIGHT_OBSERVER_H
#include <cstdint>

class preflight_observer {
public:
    virtual ~preflight_observer() = default;
    virtual void on_preflight_change(uint32_t preflight_images) = 0;
};

#endif //PREFLIGHT_OBSERVER_H
