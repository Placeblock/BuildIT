//
// Created by felix on 15.07.25.
//

#ifndef VULKANCONTEXT_H
#define VULKANCONTEXT_H

#include "engine/memory/memory_allocator.h"
#include <vulkan/vulkan.hpp>

struct queue_family_indices {
    uint32_t graphics_family;
    uint32_t present_family;
    uint32_t compute_family;
};

struct vulkan_context {
    vk::PhysicalDevice physical_device;
    vk::Device device;
    memory_allocator mem_allocator;
    queue_family_indices queue_families{};
    uint8_t preflight_frames = 0;

    vulkan_context(const vk::PhysicalDevice physical_device,
                   const vk::Device device,
                   const memory_allocator allocator)
        : physical_device(physical_device), device(device), mem_allocator(allocator) {}
};

#endif //VULKANCONTEXT_H
