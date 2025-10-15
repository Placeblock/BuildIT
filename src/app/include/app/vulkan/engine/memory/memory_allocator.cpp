//
// Created by felix on 30.09.25.
//

#include "memory_allocator.h"

#include "app/vulkan/indirect_renderer.hpp"

memory_allocator::memory_allocator(const vk::Device &device, const VmaAllocator allocator)
    : device(device), allocator(allocator) {}
