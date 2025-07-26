//
// Created by felix on 19.07.25.
//

#ifndef CIRCUITBOARD_HPP
#define CIRCUITBOARD_HPP
#include "vulkancontext.hpp"
#include <unordered_set>

class circuitboard_manager;
class imgui_circuitboard;

struct circuit_board_image {
    vk::UniqueFramebuffer framebuffer;
    vk::UniqueImageView view;
    vk::UniqueImage image;
    vk::UniqueDeviceMemory memory;

    circuit_board_image(uint32_t width,
                        uint32_t height,
                        const vulkan_context &ctx,
                        const vk::RenderPass &render_pass);
};

class circuit_board {
public:
    circuit_board(const vulkan_context &ctx,
                  const vk::RenderPass &render_pass,
                  const vk::Pipeline &pipeline,
                  const vk::DescriptorPool &descriptor_pool,
                  const vk::DescriptorSetLayout &descriptor_set_layout,
                  const vk::Sampler &sampler,
                  const vk::CommandPool &command_pool,
                  uint32_t width,
                  uint32_t height,
                  uint8_t image_count);

    void update_descriptor_set(uint32_t descriptor_image, const vk::ImageView &view) const;

    [[nodiscard]] bool pending_resize(uint32_t image_index) const;
    void resize(uint32_t image_index);
    void set_size(uint32_t width, uint32_t height);

    void record_command_buffer(uint32_t image_index);

protected:
    friend imgui_circuitboard;
    friend circuitboard_manager;
    std::vector<circuit_board_image> images;
    std::vector<vk::UniqueDescriptorSet> descriptor_sets;
    std::vector<vk::UniqueCommandBuffer> command_buffers;

    uint32_t width, height;
    uint8_t image_count;
    std::unordered_set<uint32_t> pending_resize_image_indices;

    // For RAII deconstructor and reallocation on resize
    const vulkan_context &ctx;
    const vk::Pipeline &pipeline;
    const vk::RenderPass &render_pass;
    const vk::Sampler &sampler;
    const vk::DescriptorPool &descriptor_pool;
};

#endif //CIRCUITBOARD_HPP
