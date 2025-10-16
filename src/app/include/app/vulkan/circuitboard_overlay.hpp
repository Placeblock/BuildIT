//
// Created by felix on 9/27/25.
//

#ifndef CIRCUITBOARD_OVERLAY_HPP
#define CIRCUITBOARD_OVERLAY_HPP
#include <vulkan/vulkan.hpp>

class circuitboard_overlay {
public:
    virtual ~circuitboard_overlay() = default;

    virtual void record(const vk::CommandBuffer &compute_buffer,
                        const vk::CommandBuffer &graphics_buffer,
                        uint8_t frame_index) = 0;
};

#endif //CIRCUITBOARD_OVERLAY_HPP
