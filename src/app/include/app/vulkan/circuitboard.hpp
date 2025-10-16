//
// Created by felix on 19.07.25.
//

#ifndef CIRCUITBOARD_HPP
#define CIRCUITBOARD_HPP
#include "circuitboard_overlay.hpp"
#include "vulkancontext.hpp"
#include <unordered_set>

class circuitboard_manager;
class imgui_circuitboard;

struct circuit_board_image {
    vk::UniqueFramebuffer framebuffer;
    vk::UniqueImageView view;
    UniqueVmaImage image;
    vk::UniqueDeviceMemory memory;

    circuit_board_image(uint32_t width,
                        uint32_t height,
                        const vulkan_context &ctx,
                        const vk::RenderPass &render_pass);
};

class circuit_board {
public:
    uint32_t width, height;
    std::vector<circuit_board_image> images;

    circuit_board(const vulkan_context &ctx,
                  const vk::RenderPass &render_pass,
                  const vk::Pipeline &pipeline,
                  const vk::Sampler &sampler,
                  uint32_t width,
                  uint32_t height,
                  uint8_t image_count);

    [[nodiscard]] bool pending_resize(uint32_t image_index) const;

    void resize(uint32_t image_index);

    void set_size(uint32_t new_width, uint32_t new_height);

    void record(const vk::CommandBuffer &compute_buffer,
                const vk::CommandBuffer &graphics_buffer,
                uint8_t frame_index);

    void record_command_buffer(const vk::CommandBuffer &graphics_buffer) const;

protected:
    friend imgui_circuitboard;
    friend circuitboard_manager;

    std::unordered_set<uint32_t> pending_resize_image_indices;

    // For RAII deconstructor and reallocation on resize
    const vulkan_context &ctx;
    const vk::Pipeline &pipeline;
    const vk::RenderPass &render_pass;
    const vk::Sampler &sampler;

    std::vector<circuitboard_overlay> overlays;
    uint8_t image_count;
};

#endif //CIRCUITBOARD_HPP
