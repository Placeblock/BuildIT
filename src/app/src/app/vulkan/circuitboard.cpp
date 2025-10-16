//
// Created by felix on 15.07.25.
//

#include "app/vulkan/circuitboard.hpp"

#include <iostream>
#include <memory>

circuit_board_image::circuit_board_image(const uint32_t width,
                                         const uint32_t height,
                                         const vulkan_context &ctx,
                                         const vk::RenderPass &render_pass) {

    this->image = ctx.mem_allocator.allocate_image({vk::ImageCreateFlags(),
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

    this->view = ctx.device.createImageViewUnique(vk::ImageViewCreateInfo{
        vk::ImageViewCreateFlags(),
        this->image->image,
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
                             const vk::Sampler &sampler,
                             const uint32_t width,
                             const uint32_t height,
                             const uint8_t image_count)
    : width(width)
      , height(height)
      , image_count(image_count)
      , ctx(ctx)
      , pipeline(pipeline)
      , render_pass(render_pass)
      , sampler(sampler) {
    // Create Images the circuit board is rendered onto
    for (int i = 0; i < this->image_count; ++i) {
        this->images.emplace_back(width, height, ctx, render_pass);
    }
}

bool circuit_board::pending_resize(const uint32_t image_index) const {
    return this->pending_resize_image_indices.contains(image_index);
}

void circuit_board::resize(const uint32_t image_index) {
    circuit_board_image new_image{this->width, this->height, this->ctx, this->render_pass};
    this->images[image_index] = std::move(new_image);
    this->pending_resize_image_indices.erase(image_index);
}

void circuit_board::set_size(const uint32_t new_width, const uint32_t new_height) {
    if (this->width != new_width || this->height != new_height) {
        this->width = new_width;
        this->height = new_height;
        for (int i = 0; i < this->image_count; ++i) {
            this->pending_resize_image_indices.emplace(i);
        }
    }
}

void circuit_board::record(const vk::CommandBuffer &compute_buffer,
                           const vk::CommandBuffer &graphics_buffer,
                           const uint8_t frame_index) {
    const vk::ImageMemoryBarrier barrier
        = {{},
           vk::AccessFlagBits::eColorAttachmentWrite,
           vk::ImageLayout::eUndefined,
           vk::ImageLayout::eColorAttachmentOptimal,
           this->ctx.queue_families.graphics_family,
           this->ctx.queue_families.graphics_family,
           this->images[frame_index].image->image,
           vk::ImageSubresourceRange{vk::ImageAspectFlagBits::eColor, 0, 1, 0, 1}};

    graphics_buffer.pipelineBarrier(vk::PipelineStageFlagBits::eTopOfPipe,
                                    vk::PipelineStageFlagBits::eColorAttachmentOutput,
                                    {},
                                    nullptr,
                                    nullptr,
                                    barrier);

    std::vector clearValues{vk::ClearValue{vk::ClearColorValue{0.0f, 0.0f, 0.0f, 1.0f}}};
    const vk::RenderPassBeginInfo renderPassInfo(this->render_pass,
                                                 *this->images[frame_index].framebuffer,
                                                 vk::Rect2D({0, 0},
                                                            vk::Extent2D{
                                                                this->width, this->height}),
                                                 clearValues);
    graphics_buffer.beginRenderPass(renderPassInfo, vk::SubpassContents::eInline);

    this->record_command_buffer(graphics_buffer);
    for (const auto &overlay : this->overlays) {
        overlay->record(compute_buffer, graphics_buffer, frame_index);
    }

    graphics_buffer.endRenderPass();
}

void circuit_board::record_command_buffer(const vk::CommandBuffer &graphics_buffer) const {
    graphics_buffer.bindPipeline(vk::PipelineBindPoint::eGraphics, this->pipeline);
    const vk::Viewport viewport(0.0f,
                                0.0f,
                                static_cast<float>(this->width),
                                static_cast<float>(this->height),
                                0.0f,
                                1.0f);
    graphics_buffer.setViewport(0, viewport);
    const vk::Rect2D scissor({0, 0}, vk::Extent2D{this->width, this->height});
    graphics_buffer.setScissor(0, scissor);

    graphics_buffer.draw(6, 1, 0, 0);
}

void circuit_board::add_overlay(std::unique_ptr<circuitboard_overlay> overlay) {
    this->overlays.emplace_back(std::move(overlay));
}