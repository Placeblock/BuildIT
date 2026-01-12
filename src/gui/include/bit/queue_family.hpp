//
// Created by felix on 12/25/25.
//

#ifndef BUILDIT_FIND_QUEUE_FAMILY_HPP
#define BUILDIT_FIND_QUEUE_FAMILY_HPP
#include <map>
#include <vulkan/vulkan.hpp>

namespace bit {
struct queue_family_indices_t {
    uint32_t graphics_family;
    uint32_t present_family;
    uint32_t compute_family;
    uint32_t transfer_family;
};

inline uint32_t find_simplest_queue_family(const vk::PhysicalDevice &physical_device,
                                           const vk::QueueFlagBits queue_family_type) {
    const std::vector<vk::QueueFamilyProperties> queue_family_properties = physical_device
        .getQueueFamilyProperties();

    std::multimap<int, uint32_t> candidates;

    for (uint32_t i = 0; i < queue_family_properties.size(); ++i) {
        vk::QueueFamilyProperties properties = queue_family_properties[i];
        if (!(properties.queueFlags & queue_family_type)) {
            continue;
        }
        int score = 0;
        if (properties.queueFlags & vk::QueueFlagBits::eTransfer) {
            score += 1;
        }
        if (properties.queueFlags & vk::QueueFlagBits::eGraphics) {
            score += 1;
        }
        if (properties.queueFlags & vk::QueueFlagBits::eCompute) {
            score += 1;
        }
        if (properties.queueFlags & vk::QueueFlagBits::eSparseBinding) {
            score += 1;
        }
        if (properties.queueFlags & vk::QueueFlagBits::eVideoDecodeKHR) {
            score += 1;
        }
        if (properties.queueFlags & vk::QueueFlagBits::eVideoEncodeKHR) {
            score += 1;
        }
        candidates.emplace(score, i);
    }

    return candidates.begin()->second;
}

inline uint32_t find_graphics_family(const vk::PhysicalDevice &physical_device,
                                     const vk::SurfaceKHR &surface) {
    const std::vector<vk::QueueFamilyProperties> queue_family_properties = physical_device
        .getQueueFamilyProperties();

    for (uint32_t i = 0; i < queue_family_properties.size(); ++i) {
        vk::QueueFamilyProperties properties = queue_family_properties[i];
        if ((properties.queueFlags & vk::QueueFlagBits::eGraphics) &&
            (physical_device.getSurfaceSupportKHR(i, surface))) {
            return i;
        }
    }
    for (uint32_t i = 0; i < queue_family_properties.size(); ++i) {
        vk::QueueFamilyProperties properties = queue_family_properties[i];
        if (properties.queueFlags & vk::QueueFlagBits::eGraphics) {
            return i;
        }
    }
    throw std::runtime_error("no graphics queue family found");
}
}

#endif //BUILDIT_FIND_QUEUE_FAMILY_HPP