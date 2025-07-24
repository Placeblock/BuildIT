//
// Created by felix on 19.07.25.
//

#ifndef CIRCUITBOARD_HPP
#define CIRCUITBOARD_HPP
#include "vulkancontext.hpp"

class circuitboard_manager;
class imgui_circuitboard;

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

    [[nodiscard]] uint32_t find_memory_type(uint32_t type_filter,
                                            vk::MemoryPropertyFlags properties) const;

    [[nodiscard]] std::vector<vk::UniqueImage> create_images() const;

    [[nodiscard]] vk::UniqueDeviceMemory allocate_image_memory(
        const std::vector<vk::UniqueImage> &images) const;

    [[nodiscard]] std::vector<vk::UniqueImageView> create_image_views(
        const std::vector<vk::UniqueImage> &images) const;

    void update_descriptor_sets(const std::vector<vk::UniqueImageView> &image_views) const;

    [[nodiscard]] std::vector<vk::UniqueFramebuffer> create_framebuffers(
        const std::vector<vk::UniqueImageView> &image_views) const;

    bool resize();

    void set_target_size(uint32_t width, uint32_t height);

    void record_command_buffer(uint32_t image_index);

protected:
    friend imgui_circuitboard;
    friend circuitboard_manager;
    std::vector<vk::UniqueDescriptorSet> descriptor_sets;
    std::vector<vk::UniqueFramebuffer> framebuffers;
    std::vector<vk::UniqueImage> images;
    std::vector<vk::UniqueImageView> image_views;
    std::vector<vk::UniqueCommandBuffer> command_buffers;
    vk::UniqueDeviceMemory image_memory;

    uint32_t width, height, target_width, target_height;
    uint8_t image_count;

    // For RAII deconstructor and reallocation on resize
    const vulkan_context &ctx;
    const vk::Pipeline &pipeline;
    const vk::RenderPass &render_pass;
    const vk::Sampler &sampler;
    const vk::DescriptorPool &descriptor_pool;
};

#endif //CIRCUITBOARD_HPP
