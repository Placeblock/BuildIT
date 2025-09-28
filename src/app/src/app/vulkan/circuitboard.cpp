//
// Created by felix on 15.07.25.
//

#include "app/vulkan/circuitboard.hpp"

#include "app/vulkan/memory.hpp"
#include <iostream>
#include <memory>

circuit_board_image::circuit_board_image(const uint32_t width,
                                         const uint32_t height,
                                         const vulkan_context &ctx,
                                         const vk::RenderPass &render_pass) {
    this->image = ctx.device.createImageUnique(
        vk::ImageCreateInfo{vk::ImageCreateFlags(),
                            vk::ImageType::e2D,
                            vk::Format::eR8G8B8A8Unorm,
                            {width, height, 1},
                            1,
                            1,
                            vk::SampleCountFlagBits::e1,
                            vk::ImageTiling::eOptimal,
                            vk::ImageUsageFlagBits::eColorAttachment
                                | vk::ImageUsageFlagBits::eSampled,
                            vk::SharingMode::eExclusive,
                            ctx.queue_families.graphics_family,
                            vk::ImageLayout::eUndefined});

    const vk::MemoryRequirements memory_requirements = ctx.device.getImageMemoryRequirements(
        this->image.get());
    const uint32_t image_memory_type = find_memory_type(ctx,
                                                        memory_requirements.memoryTypeBits,
                                                        vk::MemoryPropertyFlagBits::eDeviceLocal);
    const vk::MemoryAllocateInfo memory_allocate_info{memory_requirements.size, image_memory_type};
    this->memory = ctx.device.allocateMemoryUnique(memory_allocate_info);
    ctx.device.bindImageMemory(this->image.get(), this->memory.get(), 0);

    this->view = ctx.device.createImageViewUnique(vk::ImageViewCreateInfo{
        vk::ImageViewCreateFlags(),
        this->image.get(),
        vk::ImageViewType::e2D,
        vk::Format::eR8G8B8A8Unorm,
        vk::ComponentMapping{vk::ComponentSwizzle::eIdentity,
                             vk::ComponentSwizzle::eIdentity,
                             vk::ComponentSwizzle::eIdentity,
                             vk::ComponentSwizzle::eIdentity},
        vk::ImageSubresourceRange{vk::ImageAspectFlagBits::eColor, 0, 1, 0, 1}});

    this->framebuffer = ctx.device.createFramebufferUnique(
        {vk::FramebufferCreateFlags(), render_pass, this->view.get(), width, height, 1});
}

circuit_board::circuit_board(const vulkan_context &ctx,
                             const vk::RenderPass &render_pass,
                             const vk::Pipeline &pipeline,
                             const vk::DescriptorPool &descriptor_pool,
                             const vk::DescriptorSetLayout &descriptor_set_layout,
                             const vk::Sampler &sampler,
                             const vk::CommandPool &command_pool,
                             const uint32_t width,
                             const uint32_t height,
                             const uint8_t image_count)
    : width(width)
    , height(height)
    , image_count(image_count)
    , ctx(ctx)
    , pipeline(pipeline)
    , descriptor_set_layout(descriptor_set_layout)
    , render_pass(render_pass)
    , command_pool(command_pool)
    , sampler(sampler)
    , descriptor_pool(descriptor_pool) {
    // Create Images the circuit board is rendered onto
    for (int i = 0; i < this->image_count; ++i) {
        this->images.emplace_back(width, height, ctx, render_pass);
    }

    // Allocate the descriptor set for the circuit board
    std::vector layouts{this->image_count, descriptor_set_layout};
    this->descriptor_sets = ctx.device.allocateDescriptorSetsUnique(
        vk::DescriptorSetAllocateInfo{descriptor_pool, layouts});
    // Bind image views to the descriptor sets
    for (int i = 0; i < this->image_count; ++i) {
        this->update_descriptor_set(i, this->images[i].view.get());
    }

    this->command_buffers = this->ctx.device.allocateCommandBuffersUnique(
        vk::CommandBufferAllocateInfo{command_pool, vk::CommandBufferLevel::ePrimary, image_count});
    for (int i = 0; i < this->image_count; ++i) {
        this->record_command_buffer(i);
    }
}

bool circuit_board::update_in_flight_frames(const uint32_t in_flight_frames) {
    if (in_flight_frames == this->image_count)
        return false;
    this->image_count = in_flight_frames;

    // Create Images the circuit board is rendered onto
    std::vector<circuit_board_image> new_images;
    for (int i = 0; i < this->image_count; ++i) {
        new_images.emplace_back(width, height, ctx, render_pass);
    }

    this->descriptor_sets.clear();
    std::vector layouts{this->image_count, this->descriptor_set_layout};
    this->descriptor_sets = this->ctx.device.allocateDescriptorSetsUnique(
        vk::DescriptorSetAllocateInfo{this->descriptor_pool, layouts});
    // Bind image views to the descriptor sets
    for (int i = 0; i < this->image_count; ++i) {
        this->update_descriptor_set(i, new_images[i].view.get());
    }

    this->images = std::move(new_images);

    this->command_buffers.clear();
    this->command_buffers = this->ctx.device.allocateCommandBuffersUnique(
        vk::CommandBufferAllocateInfo{this->command_pool,
                                      vk::CommandBufferLevel::ePrimary,
                                      image_count});
    for (int i = 0; i < this->image_count; ++i) {
        this->record_command_buffer(i);
    }

    return true;
}

void circuit_board::update_descriptor_set(const uint32_t descriptor_image,
                                          const vk::ImageView &view) const {
    vk::DescriptorImageInfo image_info{this->sampler, view, vk::ImageLayout::eShaderReadOnlyOptimal};
    this->ctx.device
        .updateDescriptorSets(vk::WriteDescriptorSet{this->descriptor_sets[descriptor_image].get(),
                                                     0,
                                                     0,
                                                     vk::DescriptorType::eCombinedImageSampler,
                                                     image_info,
                                                     nullptr,
                                                     nullptr},
                              nullptr);
}

bool circuit_board::pending_resize(const uint32_t image_index) const {
    return this->pending_resize_image_indices.contains(image_index);
}

void circuit_board::resize(const uint32_t image_index) {
    circuit_board_image new_image{this->width, this->height, this->ctx, this->render_pass};
    this->update_descriptor_set(image_index, new_image.view.get());
    this->images[image_index] = std::move(new_image);
    this->pending_resize_image_indices.erase(image_index);
    this->record_command_buffer(image_index);
}

void circuit_board::set_size(const uint32_t width, const uint32_t height) {
    if (this->width != width || this->height != height) {
        this->width = width;
        this->height = height;
        for (int i = 0; i < this->image_count; ++i) {
            this->pending_resize_image_indices.emplace(i);
        }
    }
}

void circuit_board::record_command_buffer(const uint32_t image_index) {
    const vk::CommandBuffer &command_buffer = *this->command_buffers[image_index];
    constexpr vk::CommandBufferBeginInfo begin_info(vk::CommandBufferUsageFlags(), nullptr);
    if (command_buffer.begin(&begin_info) != vk::Result::eSuccess) {
        throw std::runtime_error("failed to begin recording circuit board command buffer");
    }

    const vk::ImageMemoryBarrier barrier
        = {{},
           vk::AccessFlagBits::eColorAttachmentWrite,
           vk::ImageLayout::eUndefined,
           vk::ImageLayout::eColorAttachmentOptimal,
           this->ctx.queue_families.graphics_family,
           this->ctx.queue_families.graphics_family,
           *this->images[image_index].image,
           vk::ImageSubresourceRange{vk::ImageAspectFlagBits::eColor, 0, 1, 0, 1}};

    command_buffer.pipelineBarrier(vk::PipelineStageFlagBits::eTopOfPipe,
                                   vk::PipelineStageFlagBits::eColorAttachmentOutput,
                                   {},
                                   nullptr,
                                   nullptr,
                                   barrier);

    std::vector clearValues{vk::ClearValue{vk::ClearColorValue{0.0f, 0.0f, 0.0f, 1.0f}}};
    const vk::RenderPassBeginInfo renderPassInfo(this->render_pass,
                                                 *this->images[image_index].framebuffer,
                                                 vk::Rect2D({0, 0},
                                                            vk::Extent2D{this->width, this->height}),
                                                 clearValues);
    command_buffer.beginRenderPass(renderPassInfo, vk::SubpassContents::eInline);
    command_buffer.bindPipeline(vk::PipelineBindPoint::eGraphics, this->pipeline);
    const vk::Viewport viewport(0.0f,
                                0.0f,
                                static_cast<float>(this->width),
                                static_cast<float>(this->height),
                                0.0f,
                                1.0f);
    command_buffer.setViewport(0, viewport);
    const vk::Rect2D scissor({0, 0}, vk::Extent2D{this->width, this->height});
    command_buffer.setScissor(0, scissor);

    command_buffer.draw(6, 1, 0, 0);

    for (auto &overlay : this->overlays) {
        overlay.record(command_buffer);
    }

    command_buffer.endRenderPass();
    command_buffer.end();
}