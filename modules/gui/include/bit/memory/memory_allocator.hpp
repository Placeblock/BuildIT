//
// Created by felix on 30.09.25.
//

#ifndef MEMORY_ALLOCATOR_H
#define MEMORY_ALLOCATOR_H

#include <vk_mem_alloc.h>
#include <spdlog/spdlog.h>
#include <vulkan/vulkan.hpp>

struct VmaImage {
    vk::Image image;

    explicit VmaImage(const vk::Image image) : image{image} {
    }

    VmaImage() = default;

    VmaImage(VmaImage const &rhs) = default;

    VmaImage &operator=(VmaImage const &rhs) = default;

    VmaImage(VmaImage &&rhs) = default;

    VmaImage &operator=(std::nullptr_t) VULKAN_HPP_NOEXCEPT {
        this->image = nullptr;
        return *this;
    }

    explicit operator vk::Image() const VULKAN_HPP_NOEXCEPT {
        return image;
    }

    explicit operator bool() const VULKAN_HPP_NOEXCEPT {
        return this->image != VK_NULL_HANDLE;
    }

    bool operator!() const VULKAN_HPP_NOEXCEPT {
        return this->image != VK_NULL_HANDLE;
    }

    auto operator<=>(VmaImage const &) const = default;
};

class VmaImageDeleter {
    VmaAllocator allocator;
    VmaAllocation allocation;

public:
    VmaImageDeleter() = default;

    VmaImageDeleter(const VmaAllocator allocator, const VmaAllocation allocation)
        : allocator{allocator}, allocation{allocation} {
    }

    VmaImageDeleter(const VmaImageDeleter &other) noexcept
        : allocator{other.allocator}, allocation{other.allocation} {
    }

    void destroy(const VmaImage image) const {
        vmaDestroyImage(this->allocator, image.image, allocation);
    }
};

template<typename Dispatch>
class vk::UniqueHandleTraits<VmaImage, Dispatch> {
public:
    using deleter = VmaImageDeleter;
};

using UniqueVmaImage = vk::UniqueHandle<VmaImage, VULKAN_HPP_DEFAULT_DISPATCHER_TYPE>;


struct VmaBuffer {
    vk::Buffer buffer;
    VmaAllocation allocation;
    VmaAllocationInfo info;

    explicit VmaBuffer(const vk::Buffer buffer,
                       const VmaAllocation allocation,
                       const VmaAllocationInfo &info) : buffer{buffer},
                                                        allocation(allocation), info(info) {
    }

    VmaBuffer() : allocation(), info() {
    };

    VmaBuffer(VmaBuffer const &rhs) = default;

    VmaBuffer &operator=(VmaBuffer const &rhs) = default;

    VmaBuffer(VmaBuffer &&rhs) = default;

    VmaBuffer &operator=(std::nullptr_t) VULKAN_HPP_NOEXCEPT {
        this->buffer = nullptr;
        this->allocation = nullptr;
        return *this;
    }

    bool operator==(const VmaBuffer &rhs) const {
        return this->buffer == rhs.buffer;
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

    auto operator<=>(VmaBuffer const &) const = default;
};

class VmaBufferDeleter {
    VmaAllocator allocator;
    VmaAllocation allocation;

public:
    VmaBufferDeleter() = default;

    VmaBufferDeleter(const VmaAllocator allocator, const VmaAllocation allocation)
        : allocator{allocator}, allocation{allocation} {
    }

    VmaBufferDeleter(const VmaBufferDeleter &other) noexcept
        : allocator{other.allocator}, allocation{other.allocation} {
    }

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


namespace bit {
class memory_allocator {
public:
    memory_allocator(const vk::Device &device, VmaAllocator allocator);

    template<typename T>
    [[nodiscard]] UniqueVmaBuffer allocate_buffer(
        const size_t size,
        const vk::BufferUsageFlags usage,
        VULKAN_HPP_NAMESPACE::ArrayProxyNoTemporaries<const uint32_t> const &queue_family_indices,
        const bool host_access = false,
        const VmaMemoryUsage memory_usage = VMA_MEMORY_USAGE_AUTO)
    const {
        auto command_buffer_info = VkBufferCreateInfo{VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO};
        command_buffer_info.size = size * sizeof(T);
        command_buffer_info.usage = static_cast<VkBufferUsageFlags>(usage);
        command_buffer_info.sharingMode = queue_family_indices.size() > 1
                                              ? VK_SHARING_MODE_CONCURRENT
                                              : VK_SHARING_MODE_EXCLUSIVE;
        command_buffer_info.queueFamilyIndexCount = queue_family_indices.size();
        command_buffer_info.pQueueFamilyIndices = queue_family_indices.data();

        VkBuffer new_buffer;
        VmaAllocationCreateInfo alloc_info = {};
        alloc_info.usage = memory_usage;
        if (host_access) {
            alloc_info.flags = VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT |
                               VMA_ALLOCATION_CREATE_MAPPED_BIT;
        }
        VmaAllocation allocation;
        vmaCreateBuffer(this->allocator,
                        &command_buffer_info,
                        &alloc_info,
                        &new_buffer,
                        &allocation,
                        nullptr);
        VmaAllocationInfo info;
        vmaGetAllocationInfo(allocator, allocation, &info);

        spdlog::debug("Allocated Buffer {}: {}", allocation->GetMemoryTypeIndex(), info.pMappedData);

        const auto deleter = VmaBufferDeleter{this->allocator, allocation};

        UniqueVmaBuffer unique_buffer{VmaBuffer{new_buffer, allocation, info}, deleter};
        return std::move(unique_buffer);
    }

    [[nodiscard]] UniqueVmaImage allocate_image(vk::ImageCreateInfo image_info) const {
        const VkImageCreateInfo c_image_info = image_info;
        VkImage new_image;
        VmaAllocationCreateInfo alloc_info = {};
        alloc_info.usage = VMA_MEMORY_USAGE_AUTO;
        VmaAllocation allocation;
        vmaCreateImage(this->allocator,
                       &c_image_info,
                       &alloc_info,
                       &new_image,
                       &allocation,
                       nullptr);

        const auto deleter = VmaImageDeleter{this->allocator, allocation};

        UniqueVmaImage uniqueImage{VmaImage{new_image}, deleter};
        return std::move(uniqueImage);
    }

private:
    const vk::Device &device;

public:
    const VmaAllocator allocator;
};
}

#endif //MEMORY_ALLOCATOR_H
