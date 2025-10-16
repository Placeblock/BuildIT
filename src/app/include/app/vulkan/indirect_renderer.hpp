//
// Created by felix on 8/3/25.
//

#ifndef INDIRECT_RENDERER_H
#define INDIRECT_RENDERER_H

#include "circuitboard.hpp"
#include "engine/pipeline/culling_pipeline.h"
#include "engine/pipeline/instanced_indirect_pipeline.h"
#include "engine/pipeline/reset_culling_pipeline.h"
#include "shader.hpp"
#include "vulkancontext.hpp"
#include "spdlog/spdlog.h"
#include <glm/glm.hpp>
#include <iostream>
#include <vulkan/vulkan.hpp>

constexpr uint32_t BUFFER_SIZE = 1032;
constexpr uint32_t WORKGROUP_SIZE = 32;

struct instance {
    float x;
    float y;
    float width;
    float height;
};

class indirect_renderer : public circuitboard_overlay {
    struct frame_resource {
        UniqueVmaBuffer visible_instances_buffer;
        UniqueVmaBuffer draw_command_buffer;

        vk::UniqueDescriptorSet culling_descriptor_set;
        vk::UniqueDescriptorSet reset_culling_descriptor_set;
        vk::UniqueDescriptorSet graphics_descriptor_set;

        frame_resource(UniqueVmaBuffer visible_instances_buffer,
                       UniqueVmaBuffer draw_command_buffer,
                       vk::UniqueDescriptorSet culling_descriptor_set,
                       vk::UniqueDescriptorSet reset_culling_descriptor_set,
                       vk::UniqueDescriptorSet graphics_descriptor_set)
            : visible_instances_buffer(std::move(visible_instances_buffer))
              , draw_command_buffer(std::move(draw_command_buffer))
              , culling_descriptor_set(std::move(culling_descriptor_set))
              , reset_culling_descriptor_set(std::move(reset_culling_descriptor_set))
              , graphics_descriptor_set(std::move(graphics_descriptor_set)) {
        }

        frame_resource(frame_resource &&other) noexcept {
            this->visible_instances_buffer = std::move(other.visible_instances_buffer);
            this->draw_command_buffer = std::move(other.draw_command_buffer);
            this->culling_descriptor_set = std::move(other.culling_descriptor_set);
            this->reset_culling_descriptor_set = std::move(other.reset_culling_descriptor_set);
            this->graphics_descriptor_set = std::move(other.graphics_descriptor_set);
        }
    };

public:
    std::vector<vk::UniqueCommandBuffer> computeCommandBuffers;
    std::vector<vk::UniqueCommandBuffer> drawCommandBuffers;

    indirect_renderer(const circuit_board &board,
                      const vk::Sampler &sampler,
                      const vulkan_context &ctx,
                      const vk::RenderPass &render_pass)
        : ctx(ctx)
          , board(board)
          , sampler(sampler)
          , culling_pipe{ctx.device}
          , reset_culling_pipe{ctx.device}
          , graphics_pipe{ctx.device, render_pass} {

        spdlog::debug("Allocating {} Bytes of memory for indirect renderer", BUFFER_SIZE);
        std::vector instances_queue_families = {ctx.queue_families.compute_family};
        this->instancesBuffer
            = ctx.mem_allocator.allocate_buffer<instance>(BUFFER_SIZE,
                                                          vk::BufferUsageFlagBits::eStorageBuffer,
                                                          instances_queue_families);

        this->allocate_frame_resources(this->ctx.preflight_frames);

        spdlog::debug("Indirect renderer created");
    }

    void allocate_frame_resources(const uint32_t preflight_images) {
        std::vector descriptor_set_layouts{*this->culling_pipe.descriptor_set_layout,
                                           *this->reset_culling_pipe.descriptor_set_layout,
                                           *this->graphics_pipe.descriptor_set_layout};
        std::vector vis_instances_queue_families = {ctx.queue_families.compute_family,
                                                    ctx.queue_families.graphics_family};

        const uint8_t set_count = 5 * preflight_images;
        std::vector pool_sizes{
            vk::DescriptorPoolSize{vk::DescriptorType::eStorageBuffer, set_count}};
        this->descriptorPool = ctx.device.createDescriptorPoolUnique(
            vk::DescriptorPoolCreateInfo(vk::DescriptorPoolCreateFlags(), set_count, pool_sizes));

        for (int i = 0; i < preflight_images; ++i) {
            UniqueVmaBuffer draw_command_buffer = ctx.mem_allocator.allocate_buffer<
                VkDrawIndirectCommand>(1,
                                       vk::BufferUsageFlagBits::eIndirectBuffer,
                                       vis_instances_queue_families);
            UniqueVmaBuffer visible_instances_buffer = ctx.mem_allocator.allocate_buffer<instance>(
                BUFFER_SIZE,
                vk::BufferUsageFlagBits::eStorageBuffer | vk::BufferUsageFlagBits::eVertexBuffer,
                vis_instances_queue_families);

            /**
             * We allocate the descriptorsets using the descriptorsetlayouts and the descriptorpool
             */
            std::vector<vk::UniqueDescriptorSet> descriptor_sets
                = ctx.device.allocateDescriptorSetsUnique(
                    vk::DescriptorSetAllocateInfo(*this->descriptorPool, descriptor_set_layouts));

            frame_resource resource{std::move(visible_instances_buffer),
                                    std::move(draw_command_buffer),
                                    std::move(descriptor_sets[0]),
                                    std::move(descriptor_sets[1]),
                                    std::move(descriptor_sets[2])};

            descriptor_sets.clear();

            this->frame_resources.push_back(std::move(resource));
        }

        vk::DescriptorBufferInfo instances_descriptor_buffer_info{
            vk::Buffer(*this->instancesBuffer),
            0,
            vk::WholeSize};
        for (int i = 0; i < preflight_images; ++i) {
            /**
             * We then tell vulkan which buffers to use for each descriptor and update the
            */
            vk::DescriptorBufferInfo visible_instances_descriptor_buffer_info{
                vk::Buffer(*this->frame_resources[i].visible_instances_buffer),
                0,
                vk::WholeSize};
            vk::DescriptorBufferInfo draw_command_descriptor_buffer_info{
                vk::Buffer(*this->frame_resources[i].draw_command_buffer), 0,
                vk::WholeSize};
            const std::vector write_descriptor_sets{
                vk::WriteDescriptorSet{*this->frame_resources[i].culling_descriptor_set,
                                       0,
                                       0,
                                       vk::DescriptorType::eStorageBuffer,
                                       nullptr,
                                       instances_descriptor_buffer_info,
                                       nullptr},
                vk::WriteDescriptorSet{*this->frame_resources[i].culling_descriptor_set,
                                       1,
                                       0,
                                       vk::DescriptorType::eStorageBuffer,
                                       nullptr,
                                       visible_instances_descriptor_buffer_info,
                                       nullptr},
                vk::WriteDescriptorSet{*this->frame_resources[i].culling_descriptor_set,
                                       2,
                                       0,
                                       vk::DescriptorType::eStorageBuffer,
                                       nullptr,
                                       draw_command_descriptor_buffer_info,
                                       nullptr},
                vk::WriteDescriptorSet{*this->frame_resources[i].reset_culling_descriptor_set,
                                       0,
                                       0,
                                       vk::DescriptorType::eStorageBuffer,
                                       nullptr,
                                       draw_command_descriptor_buffer_info,
                                       nullptr},
                vk::WriteDescriptorSet{*this->frame_resources[i].graphics_descriptor_set,
                                       0,
                                       0,
                                       vk::DescriptorType::eStorageBuffer,
                                       nullptr,
                                       visible_instances_descriptor_buffer_info,
                                       nullptr}};
            this->ctx.device.updateDescriptorSets(write_descriptor_sets, nullptr);
        }

        spdlog::debug("Allocated frame resources for indirect renderer");
    }

    void record(const vk::CommandBuffer &compute_buffer,
                const vk::CommandBuffer &graphics_buffer,
                const uint8_t frame_index) override {
        this->record_compute(compute_buffer, frame_index);
        this->record_graphics(graphics_buffer, frame_index);
    }

private:
    void record_compute(const vk::CommandBuffer &buffer, const uint8_t frame_index) {
        buffer.bindPipeline(vk::PipelineBindPoint::eCompute,
                            *this->reset_culling_pipe.pipeline.pipeline);

        buffer.bindDescriptorSets(vk::PipelineBindPoint::eCompute,
                                  *this->reset_culling_pipe.pipeline.layout,
                                  0,
                                  *this->frame_resources[frame_index].reset_culling_descriptor_set,
                                  {});
        buffer.dispatch(1, 1, 1);
        buffer.bindPipeline(vk::PipelineBindPoint::eCompute,
                            *this->reset_culling_pipe.pipeline.pipeline);

        buffer.bindDescriptorSets(vk::PipelineBindPoint::eCompute,
                                  *this->culling_pipe.pipeline.layout,
                                  0,
                                  *this->frame_resources[frame_index].culling_descriptor_set,
                                  {});
        const std::vector<instance> constants{{0, 0, 500, 500}};
        buffer.pushConstants(*this->culling_pipe.pipeline.layout,
                             vk::ShaderStageFlagBits::eCompute,
                             0,
                             constants.size() * sizeof(instance),
                             constants.data());
        buffer.dispatch(glm::ceil(BUFFER_SIZE / WORKGROUP_SIZE), 1, 1);
    }

    void record_graphics(const vk::CommandBuffer &buffer, const uint8_t frame_index) {
        buffer.bindPipeline(vk::PipelineBindPoint::eGraphics,
                            *this->graphics_pipe.pipeline.pipeline);

        buffer.bindDescriptorSets(vk::PipelineBindPoint::eGraphics,
                                  *this->graphics_pipe.pipeline.layout,
                                  0,
                                  *this->frame_resources[frame_index].graphics_descriptor_set,
                                  {});
        const std::vector draw_constants{glm::mat3(1.0f)};
        buffer.pushConstants(*this->graphics_pipe.pipeline.layout,
                             vk::ShaderStageFlagBits::eVertex,
                             0,
                             draw_constants.size() * sizeof(glm::mat3),
                             draw_constants.data());
        buffer.drawIndirect(vk::Buffer(*this->frame_resources[frame_index].draw_command_buffer),
                            0,
                            1,
                            0);
    }

    ~indirect_renderer() override {
        this->ctx.device.destroyDescriptorPool(*this->descriptorPool);
    }

    const vulkan_context &ctx;
    const circuit_board &board;
    const vk::Sampler &sampler;

    UniqueVmaBuffer instancesBuffer;

    std::vector<frame_resource> frame_resources;

    culling_pipeline culling_pipe;
    reset_culling_pipeline reset_culling_pipe;
    instanced_indirect_pipeline graphics_pipe;

    vk::UniqueRenderPass render_pass;

    vk::UniqueDescriptorPool descriptorPool;
};

#endif //INDIRECT_RENDERER_H
