//
// Created by felix on 30.09.25.
//

#include "bit/memory/memory_allocator.hpp"

using namespace bit;

memory_allocator::memory_allocator(const vk::Device &device, const VmaAllocator allocator)
    : device(device), allocator(allocator) {
}
