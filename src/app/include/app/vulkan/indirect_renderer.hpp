//
// Created by felix on 8/3/25.
//

#ifndef INDIRECT_RENDERER_H
#define INDIRECT_RENDERER_H

#include "circuitboard.hpp"
#include "engine/pipeline/culling_pipeline.h"
#include "engine/pipeline/instanced_indirect_pipeline.h"
#include "engine/pipeline/reset_culling_pipeline.h"
#include "engine/preflight_observer.h"
#include "shader.hpp"
#include "vulkancontext.hpp"
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

class indirect_renderer : public circuitboard_overlay, preflight_observer {
    struct frame_resource {
        vk::UniqueBuffer visible_instances_buffer;
        vk::UniqueBuffer draw_command_buffer;

        vk::UniqueDescriptorSet culling_descriptor_set;
        vk::UniqueDescriptorSet reset_culling_descriptor_set;
        vk::UniqueDescriptorSet graphics_descriptor_set;
    };

public:
    std::vector<vk::UniqueCommandBuffer> computeCommandBuffers;
    std::vector<vk::UniqueCommandBuffer> drawCommandBuffers;

    indirect_renderer(const circuit_board& board,
                      const vk::Sampler& sampler,
                      const vulkan_context& ctx)
        : ctx(ctx)
        , board(board)
        , sampler(sampler)
        , culling_pipe{ctx.device}
        , reset_culling_pipe{ctx.device}
        , graphics_pipe{ctx.device, *this->renderPass} {
        /**
         * We allocate the InstancesBuffer. This buffer is going to hold all instances which could be
         * visible.
         * We use UsageFlagBits of StorageBuffer, because we want to access it in the Compute Shader
         * as a SSBO. SSBO is necessary because of the amount of data.
         * We also declare which queue families are going to use the buffer. This is necessary for
         * Memory optimization
         */
        std::vector instances_queue_families = {ctx.queue_families.compute_family};
        this->instancesBuffer
            = ctx.mem_allocator.allocate_buffer<instance>(BUFFER_SIZE,
                                                          vk::BufferUsageFlagBits::eStorageBuffer,
                                                          instances_queue_families);

        /**
         * We create a color attachment for the rendered image. It uses loadop dontcare, because we
         * do not care about old data. We use storeop eStore to persist the new data.
         * For this we create a subpass which is used for graphics rendering. It uses the colorAttachment.
         */
        vk::AttachmentDescription colorAttachment(vk::AttachmentDescriptionFlags(),
                                                  vk::Format::eR8G8B8A8Unorm,
                                                  vk::SampleCountFlagBits::e1,
                                                  vk::AttachmentLoadOp::eDontCare,
                                                  vk::AttachmentStoreOp::eStore,
                                                  vk::AttachmentLoadOp::eDontCare,
                                                  vk::AttachmentStoreOp::eDontCare,
                                                  vk::ImageLayout::eColorAttachmentOptimal,
                                                  vk::ImageLayout::eColorAttachmentOptimal);
        vk::AttachmentReference colorAttachmentRef(0, vk::ImageLayout::eColorAttachmentOptimal);
        vk::SubpassDescription subpass(vk::SubpassDescriptionFlags(),
                                       vk::PipelineBindPoint::eGraphics,
                                       nullptr,
                                       colorAttachmentRef);
        /**
         * This dependency ensures, that all writes to color attachments of previous subpasses
         * are already done. This prevents subpass 0 from starting too early and potentially
         * overwriting or conflicting with previous work that touched the same images.
         */
        vk::SubpassDependency dependency(vk::SubpassExternal,
                                         0,
                                         vk::PipelineStageFlagBits::eColorAttachmentOutput,
                                         vk::PipelineStageFlagBits::eColorAttachmentOutput,
                                         vk::AccessFlagBits::eNone,
                                         vk::AccessFlagBits::eColorAttachmentWrite);
        const vk::RenderPassCreateInfo renderPassInfo(vk::RenderPassCreateFlags(),
                                                      colorAttachment,
                                                      subpass,
                                                      dependency);
        this->renderPass = this->ctx.device.createRenderPassUnique(renderPassInfo);

        this->allocate_frame_resources(this->ctx.preflight_frames);
    }

    void allocate_frame_resources(const uint32_t preflight_images) {
        std::vector descriptor_set_layouts{*this->culling_pipe.descriptor_set_layout,
                                           *this->reset_culling_pipe.descriptor_set_layout,
                                           *this->graphics_pipe.descriptor_set_layout};
        std::vector vis_instances_queue_families = {ctx.queue_families.compute_family,
                                                    ctx.queue_families.graphics_family};

        const uint8_t set_count = descriptor_set_layouts.size() * preflight_images;
        std::vector pool_sizes{
            vk::DescriptorPoolSize{vk::DescriptorType::eStorageBuffer, set_count}};
        this->descriptorPool = ctx.device.createDescriptorPoolUnique(
            vk::DescriptorPoolCreateInfo(vk::DescriptorPoolCreateFlags(), set_count, pool_sizes));

        for (int i = 0; i < preflight_images; ++i) {
            vk::UniqueBuffer draw_command_buffer = ctx.mem_allocator.allocate_buffer<
                VkDrawIndirectCommand>(1,
                                       vk::BufferUsageFlagBits::eIndirectBuffer,
                                       vis_instances_queue_families);
            vk::UniqueBuffer visible_instances_buffer = ctx.mem_allocator.allocate_buffer<instance>(
                BUFFER_SIZE,
                vk::BufferUsageFlagBits::eStorageBuffer | vk::BufferUsageFlagBits::eVertexBuffer,
                vis_instances_queue_families);
            frame_resource resource{std::move(visible_instances_buffer),
                                    std::move(draw_command_buffer)};

            /**
             * We allocate the descriptorsets using the descriptorsetlayouts and the descriptorpool
             */
            std::vector<vk::UniqueDescriptorSet> descriptor_sets
                = ctx.device.allocateDescriptorSetsUnique(
                    vk::DescriptorSetAllocateInfo(*this->descriptorPool, descriptor_set_layouts));
            resource.culling_descriptor_set = std::move(descriptor_sets[0]);
            resource.reset_culling_descriptor_set = std::move(descriptor_sets[1]);
            resource.graphics_descriptor_set = std::move(descriptor_sets[2]);

            this->frame_resources.push_back(resource);
        }
    }

    void on_preflight_change(const uint32_t preflight_images, resource_garbage& garbage) override {
        for (auto& resource : this->frame_resources) {
            garbage.mark_for_removal(std::move(resource.draw_command_buffer));
            garbage.mark_for_removal(std::move(resource.visible_instances_buffer));
        }
        this->frame_resources.clear();

        garbage.mark_for_removal(std::move(this->descriptorPool));

        this->allocate_frame_resources(preflight_images);
    }

    void record_compute(const vk::CommandBuffer& buffer, const uint8_t frame_index) {
        /**
         * We then tell vulkan which buffers to use for each descriptor and update the sets
        */
        vk::DescriptorBufferInfo instances_descriptor_buffer_info{*this->instancesBuffer,
                                                                  0,
                                                                  vk::WholeSize};
        vk::DescriptorBufferInfo visible_instances_descriptor_buffer_info{
            *this->frame_resources[frame_index].visible_instances_buffer, 0, vk::WholeSize};
        vk::DescriptorBufferInfo draw_command_descriptor_buffer_info{
            *this->frame_resources[frame_index].draw_command_buffer, 0, vk::WholeSize};
        const std::vector write_descriptor_sets{
            vk::WriteDescriptorSet{*this->frame_resources[frame_index].culling_descriptor_set,
                                   0,
                                   0,
                                   vk::DescriptorType::eStorageBuffer,
                                   nullptr,
                                   instances_descriptor_buffer_info,
                                   nullptr},
            vk::WriteDescriptorSet{*this->frame_resources[frame_index].culling_descriptor_set,
                                   1,
                                   0,
                                   vk::DescriptorType::eStorageBuffer,
                                   nullptr,
                                   visible_instances_descriptor_buffer_info,
                                   nullptr},
            vk::WriteDescriptorSet{*this->frame_resources[frame_index].culling_descriptor_set,
                                   2,
                                   0,
                                   vk::DescriptorType::eStorageBuffer,
                                   nullptr,
                                   draw_command_descriptor_buffer_info,
                                   nullptr},
            vk::WriteDescriptorSet{*this->frame_resources[frame_index].reset_culling_descriptor_set,
                                   0,
                                   0,
                                   vk::DescriptorType::eStorageBuffer,
                                   nullptr,
                                   draw_command_descriptor_buffer_info,
                                   nullptr}};
        this->ctx.device.updateDescriptorSets(write_descriptor_sets, nullptr);

        buffer.begin(vk::CommandBufferBeginInfo());
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
        buffer.end();
    }

    void record_graphics(const vk::CommandBuffer& buffer, const uint8_t frame_index) {
        /**
         * We then tell vulkan which buffers to use for each descriptor and update the sets
         */
        vk::DescriptorBufferInfo visibleInstancesDescriptorBufferInfo{
            *this->frame_resources[frame_index].visible_instances_buffer, 0, vk::WholeSize};
        const std::vector write_descriptor_sets{
            vk::WriteDescriptorSet{*this->frame_resources[frame_index].graphics_descriptor_set,
                                   0,
                                   0,
                                   vk::DescriptorType::eStorageBuffer,
                                   nullptr,
                                   visibleInstancesDescriptorBufferInfo,
                                   nullptr}};
        this->ctx.device.updateDescriptorSets(write_descriptor_sets, nullptr);

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
        buffer.drawIndirect(*this->frame_resources[frame_index].draw_command_buffer, 0, 1, 0);
    }

    ~indirect_renderer() override {
        this->ctx.device.destroyDescriptorPool(*this->descriptorPool);
    }

private:
    const vulkan_context& ctx;
    const circuit_board& board;
    const vk::Sampler& sampler;

    vk::UniqueBuffer instancesBuffer;

    std::vector<frame_resource> frame_resources;

    culling_pipeline culling_pipe;
    reset_culling_pipeline reset_culling_pipe;
    instanced_indirect_pipeline graphics_pipe;

    vk::UniqueRenderPass renderPass;

    vk::UniqueDescriptorPool descriptorPool;
};

#endif //INDIRECT_RENDERER_H
