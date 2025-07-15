//
// Created by felix on 15.07.25.
//

#ifndef VULKANCONTEXT_H
#define VULKANCONTEXT_H

#include <vulkan/vulkan.hpp>

struct queue_family_indices {
    uint32_t graphics_family;
    uint32_t present_family;
};

struct vulkan_context {
    vk::Device device;
    vk::PhysicalDevice physical_device;
    queue_family_indices queue_family_indices;
};

#endif //VULKANCONTEXT_H
