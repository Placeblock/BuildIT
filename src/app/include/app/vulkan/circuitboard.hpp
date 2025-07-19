//
// Created by felix on 19.07.25.
//

#ifndef CIRCUITBOARD_HPP
#define CIRCUITBOARD_HPP
#include "vulkancontext.hpp"

class imgui_circuitboard;

class circuit_board {
public:
    vk::UniqueSemaphore render_finished_semaphore;

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

    void create_images();

    void allocate_image_memory();

    void create_image_views();

    void update_descriptor_sets() const;

    void create_framebuffers();

    bool resize(int width, int height);

    void record_command_buffer(uint32_t image_index);

    void render(const vk::Queue &queue, uint32_t image_index);

protected:
    friend imgui_circuitboard;
    std::vector<vk::UniqueDescriptorSet> descriptor_sets;
    std::vector<vk::UniqueFramebuffer> framebuffers;
    std::vector<vk::UniqueImage> images;
    std::vector<vk::UniqueImageView> image_views;
    std::vector<vk::UniqueCommandBuffer> command_buffers;
    vk::UniqueDeviceMemory image_memory;

    std::vector<vk::UniqueFence> in_flight_fences;
    bool image_resize_needed;
    uint32_t width, height;
    uint8_t image_count;

    // For RAII deconstructor and reallocation on resize
    const vulkan_context &ctx;
    const vk::Pipeline &pipeline;
    const vk::RenderPass &render_pass;
    const vk::Sampler &sampler;
    const vk::DescriptorPool &descriptor_pool;
};

#endif //CIRCUITBOARD_HPP
