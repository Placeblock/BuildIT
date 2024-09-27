//
// Created by felix on 9/27/24.
//

#ifndef BUILDIT_COUNTER_H
#define BUILDIT_COUNTER_H

#include <cstdint>
#include <atomic>

class Counter {
private:
    std::atomic<uint32_t> count;
public:
    [[nodiscard]] uint32_t increaseAndGet();
};


#endif //BUILDIT_COUNTER_H
