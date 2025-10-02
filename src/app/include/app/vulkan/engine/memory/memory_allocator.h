//
// Created by felix on 30.09.25.
//

#ifndef MEMORY_ALLOCATOR_H
#define MEMORY_ALLOCATOR_H
#include "vma/vk_mem_alloc.h"
#include <vulkan/vulkan.hpp>

struct VmaBufferDeleter {
    VmaAllocator allocator;
    VmaAllocation allocation;

    void operator()(const vk::Buffer buffer) const {
        vmaDestroyBuffer(allocator, buffer, allocation);
    }
};

class memory_allocator {
public:
    memory_allocator(const vk::Device& device, const VmaAllocator& allocator);

    template<typename T>
    [[nodiscard]] vk::UniqueBuffer allocate_buffer(
        const size_t size,
        const vk::BufferUsageFlags usage,
        VULKAN_HPP_NAMESPACE::ArrayProxyNoTemporaries<const uint32_t> const& queue_family_indices)
        const {
        auto command_buffer_info = VkBufferCreateInfo{VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO};
        command_buffer_info.size = size * sizeof(T);
        command_buffer_info.usage = static_cast<VkBufferUsageFlags>(usage);
        command_buffer_info.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
        command_buffer_info.queueFamilyIndexCount = queue_family_indices.size();
        command_buffer_info.pQueueFamilyIndices = queue_family_indices.data();

        VkBuffer new_buffer;
        VmaAllocationCreateInfo allocInfo = {};
        allocInfo.usage = VMA_MEMORY_USAGE_AUTO;
        VmaAllocation allocation;
        vmaCreateBuffer(this->allocator,
                        &command_buffer_info,
                        &allocInfo,
                        &new_buffer,
                        &allocation,
                        nullptr);

        auto deleter = [this, allocation](const vk::Buffer buffer) {
            vmaDestroyBuffer(this->allocator, buffer, allocation);
        };

        vk::UniqueBuffer uniqueBuffer(new_buffer, deleter);
        return std::move(uniqueBuffer);
    }

private:
    const vk::Device& device;
    const VmaAllocator& allocator;
};

#endif //MEMORY_ALLOCATOR_H
