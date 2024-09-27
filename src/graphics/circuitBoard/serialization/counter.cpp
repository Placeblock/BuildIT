//
// Created by felix on 9/27/24.
//

#include "counter.h"

uint32_t Counter::increaseAndGet() {
    this->count++;
    return this->count;
}
