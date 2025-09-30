//
// Created by felix on 15.07.25.
//

#ifndef VULKANCONTEXT_H
#define VULKANCONTEXT_H

#include "engine/memory/memory_allocator.h"
#include "vma/vk_mem_alloc.h"
#include <vulkan/vulkan.hpp>

struct queue_family_indices {
    uint32_t graphics_family;
    uint32_t present_family;
    uint32_t compute_family;
};

struct vulkan_context {
    vk::Device device;
    memory_allocator mem_allocator;
    vk::PhysicalDevice physical_device;
    queue_family_indices queue_families;
};

#endif //VULKANCONTEXT_H
