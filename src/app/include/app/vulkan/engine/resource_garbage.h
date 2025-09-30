//
// Created by felix on 30.09.25.
//

#ifndef RESOURCE_GARBAGE_H
#define RESOURCE_GARBAGE_H
#include <memory>
#include <ranges>
#include <vulkan/vulkan.hpp>

class resource_garbage {
    struct resource_entry {
        uint64_t destroyed_serial;
    };

    template<typename Type, typename Dispatch>
    struct unique_resource_entry : resource_entry {
        vk::UniqueHandle<Type, Dispatch> resource;
    };

public:
    resource_garbage(const vk::Device &device, uint64_t &current_serial)
        : device(device), current_serial(current_serial) {}

    template<typename Type, typename Dispatch>
    void mark_for_removal(vk::UniqueHandle<Type, Dispatch> handle) {
        this->per_frame_resources.push_back(
            std::make_unique<unique_resource_entry>(this->current_serial, std::move(handle)));
    }

    void destroy_unused_resources() {
        uint64_t completed = device.getSemaphoreCounterValue(timeline);
        std::erase_if(this->per_frame_resources, [completed](const auto &entry) {
            return entry->destroyed_serial <= completed;
        });
    }

private:
    const vk::Device &device;
    vk::Semaphore timeline;
    uint64_t &current_serial; // externally incremented per submission
    std::vector<std::unique_ptr<resource_entry>> per_frame_resources;
};

#endif //RESOURCE_GARBAGE_H
