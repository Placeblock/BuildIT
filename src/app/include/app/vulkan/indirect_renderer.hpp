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
#include "memory.hpp"
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
    };

public:
    std::vector<vk::UniqueCommandBuffer> computeCommandBuffers;
    std::vector<vk::UniqueCommandBuffer> drawCommandBuffers;

    indirect_renderer(const circuit_board& board,
                      const vk::Sampler& sampler,
                      const vulkan_context& ctx)
        : ctx(ctx), board(board), sampler(sampler) {
        /**
         * We instantiate the Command Pool. This Command Pool is going to later be used to instantiate the
         * CommandBuffers used for the compute stage
        */
        this->drawCommandPool = ctx.device.createCommandPoolUnique(
            vk::CommandPoolCreateInfo{vk::CommandPoolCreateFlagBits::eResetCommandBuffer,
                                      this->ctx.queue_families.graphics_family});
        this->computeCommandPool = ctx.device.createCommandPoolUnique(
            vk::CommandPoolCreateInfo{vk::CommandPoolCreateFlagBits::eResetCommandBuffer,
                                      this->ctx.queue_families.compute_family});

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

        this->culling_pipe = culling_pipeline{ctx.device};
        this->reset_culling_pipe = reset_culling_pipeline{ctx.device};
        this->instanced_indirect_pipe = instanced_indirect_pipeline{ctx.device, *this->renderPass};

        /**
         * Now that we have created the layouts for everything and pipelines, we can start instantiating
         * the individual buffers.
         */
        std::vector pool_sizes{vk::DescriptorPoolSize{vk::DescriptorType::eStorageBuffer, 5}};
        this->descriptorPool = ctx.device.createDescriptorPoolUnique(
            vk::DescriptorPoolCreateInfo(vk::DescriptorPoolCreateFlags(), 5, pool_sizes));

        /**
         * We allocate the descriptorsets using the descriptorsetlayouts and the descriptorpool
         */
        std::vector descriptorSetLayouts{*this->culling_pipe.descriptor_set_layout,
                                         *this->reset_culling_pipe.descriptor_set_layout,
                                         *this->instanced_indirect_pipe.descriptor_set_layout};
        std::vector<vk::UniqueDescriptorSet> descriptor_sets
            = ctx.device.allocateDescriptorSetsUnique(
                vk::DescriptorSetAllocateInfo(*this->descriptorPool, descriptorSetLayouts));
        this->cullingDescriptorSet = std::move(descriptor_sets[0]);
        this->resetCullingDescriptorSet = std::move(descriptor_sets[1]);
        this->instancedIndirectDescriptorSet = std::move(descriptor_sets[2]);

        /**
         * We then tell vulkan which buffers to use for each descriptor and update the sets
         */
        vk::DescriptorBufferInfo instancesDescriptorBufferInfo{*this->instancesBuffer,
                                                               0,
                                                               vk::WholeSize};
        vk::DescriptorBufferInfo visibleInstancesDescriptorBufferInfo{*this->instancesBuffer,
                                                                      0,
                                                                      vk::WholeSize};
        vk::DescriptorBufferInfo indirectDescriptorBufferInfo{*this->indirectDrawBuffer,
                                                              0,
                                                              vk::WholeSize};
        std::vector writeDescriptorSets{vk::WriteDescriptorSet{*this->cullingDescriptorSet,
                                                               0,
                                                               0,
                                                               vk::DescriptorType::eStorageBuffer,
                                                               nullptr,
                                                               instancesDescriptorBufferInfo,
                                                               nullptr},
                                        vk::WriteDescriptorSet{*this->cullingDescriptorSet,
                                                               1,
                                                               0,
                                                               vk::DescriptorType::eStorageBuffer,
                                                               nullptr,
                                                               visibleInstancesDescriptorBufferInfo,
                                                               nullptr},
                                        vk::WriteDescriptorSet{*this->cullingDescriptorSet,
                                                               2,
                                                               0,
                                                               vk::DescriptorType::eStorageBuffer,
                                                               nullptr,
                                                               indirectDescriptorBufferInfo,
                                                               nullptr},
                                        vk::WriteDescriptorSet{*this->resetCullingDescriptorSet,
                                                               0,
                                                               0,
                                                               vk::DescriptorType::eStorageBuffer,
                                                               nullptr,
                                                               indirectDescriptorBufferInfo,
                                                               nullptr},
                                        vk::WriteDescriptorSet{*this->instancedIndirectDescriptorSet,
                                                               0,
                                                               0,
                                                               vk::DescriptorType::eStorageBuffer,
                                                               nullptr,
                                                               visibleInstancesDescriptorBufferInfo,
                                                               nullptr}};
        this->ctx.device.updateDescriptorSets(writeDescriptorSets, nullptr);

        /**
         * We allocate draw and compute command buffers for each image
         */
        this->drawCommandBuffers = std::move(ctx.device.allocateCommandBuffersUnique(
            vk::CommandBufferAllocateInfo(*this->drawCommandPool,
                                          vk::CommandBufferLevel::ePrimary,
                                          board.image_count)));
        this->computeCommandBuffers = std::move(ctx.device.allocateCommandBuffersUnique(
            vk::CommandBufferAllocateInfo(*this->computeCommandPool,
                                          vk::CommandBufferLevel::ePrimary,
                                          board.image_count)));

        /**
         * Record the individual command buffers
         */
        for (int i = 0; i < this->board.image_count; ++i) {
            this->recordCommandBuffer(i);
        }
    }

    void allocate_frame_resources(const uint32_t preflight_images) {
        std::vector vis_instances_queue_families = {ctx.queue_families.compute_family,
                                                    ctx.queue_families.graphics_family};

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
            this->frame_resources.push_back(resource);
        }
    }

    void on_preflight_change(const uint32_t preflight_images, resource_garbage& garbage) override {
        for (auto [visible_instances_buffer, draw_command_buffer] : this->frame_resources) {
            garbage.mark_for_removal(std::move(draw_command_buffer));
            garbage.mark_for_removal(std::move(visible_instances_buffer));
        }
        this->frame_resources.clear();

        this->allocate_frame_resources(preflight_images);
    }

    void recordCommandBuffer(const uint8_t frame) {
        this->computeCommandBuffers[frame]->begin(vk::CommandBufferBeginInfo());
        this->computeCommandBuffers[frame]->bindPipeline(vk::PipelineBindPoint::eCompute,
                                                         *this->computePipeline);

        this->computeCommandBuffers[frame]->bindDescriptorSets(vk::PipelineBindPoint::eCompute,
                                                               *this->computePipelineLayout,
                                                               0,
                                                               *this->computeDescriptorSet,
                                                               {});
        const std::vector<instance> constants{{0, 0, 500, 500}};
        this->computeCommandBuffers[frame]->pushConstants(*this->computePipelineLayout,
                                                          vk::ShaderStageFlagBits::eCompute,
                                                          0,
                                                          constants.size() * sizeof(instance),
                                                          constants.data());
        this->computeCommandBuffers[frame]->dispatch(glm::ceil(BUFFER_SIZE / WORKGROUP_SIZE), 1, 1);
        this->computeCommandBuffers[frame]->end();

        this->drawCommandBuffers[frame]->begin(vk::CommandBufferBeginInfo());
        std::vector clearValues{vk::ClearValue{vk::ClearColorValue{0.0f, 0.0f, 0.0f, 1.0f}}};
        const vk::RenderPassBeginInfo beginRenderPassInfo(*this->renderPass,
                                                          *board.images[frame].framebuffer,
                                                          vk::Rect2D({0, 0},
                                                                     vk::Extent2D{board.width,
                                                                                  board.height}),
                                                          clearValues);
        this->drawCommandBuffers[frame]->beginRenderPass(beginRenderPassInfo,
                                                         vk::SubpassContents::eInline);
        this->drawCommandBuffers[frame]->bindPipeline(vk::PipelineBindPoint::eGraphics,
                                                      *this->drawPipeline);

        this->drawCommandBuffers[frame]->bindDescriptorSets(vk::PipelineBindPoint::eGraphics,
                                                            *this->drawPipelineLayout,
                                                            0,
                                                            *this->instancedIndirectDescriptorSet,
                                                            {});
        const vk::Viewport viewport(0.0f,
                                    0.0f,
                                    static_cast<float>(this->board.width),
                                    static_cast<float>(this->board.height),
                                    0.0f,
                                    1.0f);
        this->drawCommandBuffers[frame]->setViewport(0, viewport);
        const vk::Rect2D scissor({0, 0}, vk::Extent2D{this->board.width, this->board.height});
        this->drawCommandBuffers[frame]->setScissor(0, scissor);

        const std::vector draw_constants{glm::mat3(1.0f)};
        this->drawCommandBuffers[frame]->pushConstants(*this->drawPipelineLayout,
                                                       vk::ShaderStageFlagBits::eVertex,
                                                       0,
                                                       draw_constants.size() * sizeof(glm::mat3),
                                                       draw_constants.data());
        this->drawCommandBuffers[frame]->draw(6, BUFFER_SIZE, 0, 0);

        this->drawCommandBuffers[frame]->endRenderPass();
        this->drawCommandBuffers[frame]->end();
    }

    void cull(const vk::Queue& queue) {}

    void record(const vk::CommandBuffer& buffer) {
        buffer.bindPipeline(vk::PipelineBindPoint::eGraphics, *this->drawPipe);

        buffer.bindDescriptorSets(vk::PipelineBindPoint::eGraphics,
                                  *this->drawPipelineLayout,
                                  0,
                                  *this->instancedIndirectDescriptorSet,
                                  {});
        const std::vector draw_constants{glm::mat3(1.0f)};
        buffer.pushConstants(*this->drawPipelineLayout,
                             vk::ShaderStageFlagBits::eVertex,
                             0,
                             draw_constants.size() * sizeof(glm::mat3),
                             draw_constants.data());
        buffer.drawIndirectCount() buffer.draw(6, BUFFER_SIZE, 0, 0);
    }

    ~indirect_renderer() {
        this->ctx.device.freeDescriptorSets(*this->descriptorPool, *this->cullingDescriptorSet);
        this->ctx.device.freeDescriptorSets(*this->descriptorPool, *this->resetCullingDescriptorSet);
        this->ctx.device.freeDescriptorSets(*this->descriptorPool,
                                            *this->instancedIndirectDescriptorSet);
    }

private:
    const vulkan_context& ctx;
    const circuit_board& board;
    const vk::Sampler& sampler;

    vk::UniqueCommandPool computeCommandPool;
    vk::UniqueCommandPool drawCommandPool;

    vk::UniqueBuffer instancesBuffer;

    std::vector<frame_resource> frame_resources;

    culling_pipeline culling_pipe;
    reset_culling_pipeline reset_culling_pipe;
    instanced_indirect_pipeline instanced_indirect_pipe;

    vk::UniqueRenderPass renderPass;

    vk::UniqueDescriptorPool descriptorPool;
    vk::UniqueDescriptorSet cullingDescriptorSet;
    vk::UniqueDescriptorSet resetCullingDescriptorSet;
    vk::UniqueDescriptorSet instancedIndirectDescriptorSet;
};

#endif //INDIRECT_RENDERER_H
