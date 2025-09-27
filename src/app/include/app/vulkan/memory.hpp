//
// Created by felix on 9/27/25.
//

#ifndef MEMORY_HPP
#define MEMORY_HPP
#include "vulkancontext.hpp"

static uint32_t find_memory_type(const vulkan_context &ctx,
                                 const uint32_t type_filter,
                                 const vk::MemoryPropertyFlags properties) {
    const vk::PhysicalDeviceMemoryProperties mem_properties = ctx.physical_device
                                                                  .getMemoryProperties();
    for (uint32_t i = 0; i < mem_properties.memoryTypeCount; i++) {
        if ((type_filter & (1 << i))
            && (mem_properties.memoryTypes[i].propertyFlags & properties) == properties) {
            return i;
        }
    }
    throw std::runtime_error("failed to find suitable memory type!");
}

#endif //MEMORY_HPP
