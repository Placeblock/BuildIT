//
// Created by felix on 30.09.25.
//

#ifndef DYNAMIC_SIZE_BUFFER_H
#define DYNAMIC_SIZE_BUFFER_H
#include <vulkan/vulkan.hpp>

template<typename T>
class dynamic_size_buffer {
public:
    virtual ~dynamic_size_buffer() = default;
    explicit dynamic_size_buffer(const vk::Device& device,
                                 const size_t capacity,
                                 const size_t size = 0)
        : device(device), size(size), capacity(capacity) {}

    virtual vk::UniqueBuffer allocate_buffer(size_t capacity) = 0;
    virtual vk::UniqueDeviceMemory allocate_memory(size_t capacity) = 0;

    void update_size(const size_t new_size, const vk::CommandBuffer& command_buffer) {
        const float level = static_cast<float>(new_size) / this->capacity;
        if (level > 0.9f) {
            this->grow(command_buffer);
            return;
        }
        if (level < 0.5f) {
            this->shrink(command_buffer);
        }
        this->size = new_size;
    }

private:
    void grow(const size_t new_size, const vk::CommandBuffer& command_buffer) {
        const size_t new_capacity = std::max(this->capacity * 1.5, new_size / 0.7);
        this->reallocate(new_capacity, command_buffer);
    }
    void shrink(const size_t new_size, const vk::CommandBuffer& command_buffer) {
        const size_t new_capacity = std::min(this->capacity / 2, new_size * 1.9);
        this->reallocate(new_capacity, command_buffer);
    }

    void reallocate(const size_t new_capacity, const vk::CommandBuffer& command_buffer) {
        vk::UniqueBuffer new_buffer = allocate_buffer(new_capacity);
        vk::UniqueDeviceMemory new_memory = allocate_memory(new_capacity);

        this->device.bindBufferMemory(*new_buffer, *new_memory, 0);

        const vk::BufferCopy copy_region{0, 0, this->size};
        command_buffer.copyBuffer(*this->buffer, *new_buffer, copy_region);

        this->buffer = std::move(new_buffer);
        this->memory = std::move(new_memory);
        this->capacity = new_capacity;
    }

    const vk::Device& device;

    size_t size;
    size_t capacity;

    vk::UniqueBuffer buffer;
    vk::UniqueDeviceMemory memory;
};

#endif //DYNAMIC_SIZE_BUFFER_H
