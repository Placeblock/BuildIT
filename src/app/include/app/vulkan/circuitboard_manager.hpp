//
// Created by felix on 18.07.25.
//

#ifndef CIRCUITBOARD_MANAGER_H
#define CIRCUITBOARD_MANAGER_H
#include "app/vulkan/vulkancontext.hpp"
#include "circuitboard.hpp"
#include <memory>
#include <vulkan/vulkan.hpp>

class circuitboard_manager {
public:
    explicit circuitboard_manager(const vulkan_context &ctx, uint32_t in_flight_frames);

    vk::UniqueSampler sampler;

    circuit_board *create_board();

    void render(const vk::Queue &compute_queue,
                const vk::Queue &graphics_queue,
                uint32_t in_flight_frame);

    [[nodiscard]] bool can_resize(uint32_t image_index);

private:
    struct frame_resource {
        vk::UniqueFence preflight_fence;
        vk::UniqueSemaphore compute_finished_semaphore;
        vk::UniqueSemaphore render_finished_semaphore;
        vk::UniqueCommandBuffer graphics_buffer;
        vk::UniqueCommandBuffer compute_buffer;

        frame_resource(vk::UniqueFence preflight_fence,
                       vk::UniqueSemaphore compute_finished_semaphore,
                       vk::UniqueSemaphore render_finished_semaphore,
                       vk::UniqueCommandBuffer graphics_buffer,
                       vk::UniqueCommandBuffer compute_buffer)
            : preflight_fence(std::move(preflight_fence))
              , compute_finished_semaphore(std::move(compute_finished_semaphore))
              , render_finished_semaphore(std::move(render_finished_semaphore))
              , graphics_buffer(std::move(graphics_buffer))
              , compute_buffer(std::move(compute_buffer)) {
        }

        frame_resource(frame_resource &&other) noexcept {
            this->preflight_fence = std::move(other.preflight_fence);
            this->compute_finished_semaphore = std::move(other.compute_finished_semaphore);
            this->render_finished_semaphore = std::move(other.render_finished_semaphore);
            this->graphics_buffer = std::move(other.graphics_buffer);
            this->compute_buffer = std::move(other.compute_buffer);
        }
    };

    void create_sampler();

    void create_command_pool();

    void create_render_pass();

    void create_pipeline();

public:
    vk::UniqueRenderPass render_pass;
    std::vector<frame_resource> frame_resources;

private:
    vk::UniqueCommandPool graphics_command_pool;
    vk::UniqueCommandPool compute_command_pool;
    vk::UniquePipelineLayout pipeline_layout;
    vk::UniquePipeline pipeline;
    uint32_t in_flight_frames;

    const vulkan_context &ctx;

    std::vector<std::unique_ptr<circuit_board> > circuit_boards;
};

#endif //CIRCUITBOARD_MANAGER_H
