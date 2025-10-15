//
// Created by felix on 30.09.25.
//

#ifndef MEMORY_ALLOCATOR_H
#define MEMORY_ALLOCATOR_H
#include "vk_mem_alloc.h"
#include <vulkan/vulkan.hpp>

struct VmaBuffer {
    vk::Buffer buffer;

    explicit VmaBuffer(const vk::Buffer buffer) : buffer{buffer} {}
    VmaBuffer() = default;
    VmaBuffer(VmaBuffer const& rhs) = default;
    VmaBuffer& operator=(VmaBuffer const& rhs) = default;
    VmaBuffer(VmaBuffer&& rhs) = default;
    VmaBuffer& operator=(std::nullptr_t) VULKAN_HPP_NOEXCEPT {
        this->buffer = nullptr;
        return *this;
    }

    explicit operator vk::Buffer() const VULKAN_HPP_NOEXCEPT {
        return buffer;
    }

    explicit operator bool() const VULKAN_HPP_NOEXCEPT {
        return this->buffer != VK_NULL_HANDLE;
    }
    bool operator!() const VULKAN_HPP_NOEXCEPT {
        return this->buffer != VK_NULL_HANDLE;
    }
    auto operator<=>(VmaBuffer const&) const = default;
};

class VmaBufferDeleter {
    VmaAllocator allocator;
    VmaAllocation allocation;

public:
    VmaBufferDeleter() = default;

    VmaBufferDeleter(const VmaAllocator allocator, const VmaAllocation allocation)
        : allocator{allocator}, allocation{allocation} {}

    VmaBufferDeleter(const VmaBufferDeleter& other) noexcept
        : allocator{other.allocator}, allocation{other.allocation} {}

    void destroy(const VmaBuffer buffer) const {
        vmaDestroyBuffer(this->allocator, buffer.buffer, allocation);
    }
};

template<typename Dispatch>
class vk::UniqueHandleTraits<VmaBuffer, Dispatch> {
public:
    using deleter = VmaBufferDeleter;
};

using UniqueVmaBuffer = vk::UniqueHandle<VmaBuffer, VULKAN_HPP_DEFAULT_DISPATCHER_TYPE>;

class memory_allocator {
public:
    memory_allocator(const vk::Device& device, VmaAllocator allocator);

    template<typename T>
    [[nodiscard]] UniqueVmaBuffer allocate_buffer(
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

        const auto deleter = VmaBufferDeleter{this->allocator, allocation};

        UniqueVmaBuffer uniqueBuffer{VmaBuffer{new_buffer}, deleter};
        return std::move(uniqueBuffer);
    }

private:
    const vk::Device& device;

public:
    const VmaAllocator allocator;
};

#endif //MEMORY_ALLOCATOR_H
