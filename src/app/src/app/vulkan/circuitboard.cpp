//
// Created by felix on 15.07.25.
//

#include "app/vulkan/circuitboard.hpp"

#include <iostream>
#include <memory>

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
    , target_width(width)
    , height(height)
    , target_height(height)
    , image_count(image_count)
    , ctx(ctx)
    , render_pass(render_pass)
    , pipeline(pipeline)
    , sampler(sampler)
    , descriptor_pool(descriptor_pool) {
    // Create Images the circuit board is rendered onto
    this->images = std::move(this->create_images());
    // Create Memory for the Images and bind them to the memory
    this->image_memory = this->allocate_image_memory(this->images);
    // Create Image Views for the Images
    this->image_views = this->create_image_views(this->images);
    // Allocate the descriptor set for the circuit board
    std::vector layouts = {descriptor_set_layout, descriptor_set_layout, descriptor_set_layout};
    this->descriptor_sets = ctx.device.allocateDescriptorSetsUnique(
        vk::DescriptorSetAllocateInfo{descriptor_pool, layouts});
    // Bind image views to the descriptor sets
    this->update_descriptor_sets(this->image_views);
    this->framebuffers = std::move(this->create_framebuffers(this->image_views));

    this->command_buffers = this->ctx.device.allocateCommandBuffersUnique(
        vk::CommandBufferAllocateInfo{command_pool, vk::CommandBufferLevel::ePrimary, image_count});
    for (int i = 0; i < this->image_count; ++i) {
        this->record_command_buffer(i);
    }
}

uint32_t circuit_board::find_memory_type(const uint32_t type_filter,
                                         const vk::MemoryPropertyFlags properties) const {
    const vk::PhysicalDeviceMemoryProperties mem_properties = this->ctx.physical_device
                                                                  .getMemoryProperties();
    for (uint32_t i = 0; i < mem_properties.memoryTypeCount; i++) {
        if ((type_filter & (1 << i))
            && (mem_properties.memoryTypes[i].propertyFlags & properties) == properties) {
            return i;
        }
    }
    throw std::runtime_error("failed to find suitable memory type!");
}

std::vector<vk::UniqueImage> circuit_board::create_images() const {
    std::vector queue_family_indices = {this->ctx.queue_families.graphics_family};
    std::vector<vk::UniqueImage> images;
    for (int i = 0; i < this->image_count; ++i) {
        images.push_back(std::move(this->ctx.device.createImageUnique(
            vk::ImageCreateInfo{vk::ImageCreateFlags(),
                                vk::ImageType::e2D,
                                vk::Format::eR8G8B8A8Unorm,
                                {this->width, this->height, 1},
                                1,
                                1,
                                vk::SampleCountFlagBits::e1,
                                vk::ImageTiling::eOptimal,
                                vk::ImageUsageFlagBits::eColorAttachment
                                    | vk::ImageUsageFlagBits::eSampled,
                                vk::SharingMode::eExclusive,
                                queue_family_indices,
                                vk::ImageLayout::eUndefined})));
    }
    return images;
}

vk::UniqueDeviceMemory circuit_board::allocate_image_memory(
    const std::vector<vk::UniqueImage> &images) const {
    vk::DeviceSize total_size = 0;
    vk::DeviceSize offsets[this->image_count];
    uint32_t image_memory_type;
    for (int i = 0; i < this->image_count; ++i) {
        const vk::MemoryRequirements memory_requirements
            = this->ctx.device.getImageMemoryRequirements(images[i].get());
        offsets[i] = (total_size + memory_requirements.alignment - 1)
                     & ~(memory_requirements.alignment - 1);
        total_size = offsets[i] + memory_requirements.size;
        if (i == 0) {
            image_memory_type = this->find_memory_type(memory_requirements.memoryTypeBits,
                                                       vk::MemoryPropertyFlagBits::eDeviceLocal);
        }
    }
    const vk::MemoryAllocateInfo memory_allocate_info{total_size, image_memory_type};
    vk::UniqueDeviceMemory memory = this->ctx.device.allocateMemoryUnique(memory_allocate_info);
    for (int i = 0; i < this->image_count; ++i) {
        this->ctx.device.bindImageMemory(images[i].get(), memory.get(), offsets[i]);
    }
    return memory;
}

std::vector<vk::UniqueImageView> circuit_board::create_image_views(
    const std::vector<vk::UniqueImage> &images) const {
    // Create Image Views
    std::vector<vk::UniqueImageView> image_views;
    for (int i = 0; i < this->image_count; ++i) {
        image_views.push_back(
            std::move(this->ctx.device.createImageViewUnique(vk::ImageViewCreateInfo{
                vk::ImageViewCreateFlags(),
                images[i].get(),
                vk::ImageViewType::e2D,
                vk::Format::eR8G8B8A8Unorm,
                vk::ComponentMapping{vk::ComponentSwizzle::eIdentity,
                                     vk::ComponentSwizzle::eIdentity,
                                     vk::ComponentSwizzle::eIdentity,
                                     vk::ComponentSwizzle::eIdentity},
                vk::ImageSubresourceRange{vk::ImageAspectFlagBits::eColor, 0, 1, 0, 1}})));
    }
    return image_views;
}

void circuit_board::update_descriptor_sets(
    const std::vector<vk::UniqueImageView> &image_views) const {
    for (int i = 0; i < this->image_count; ++i) {
        vk::DescriptorImageInfo image_info{this->sampler,
                                           image_views[i].get(),
                                           vk::ImageLayout::eShaderReadOnlyOptimal};
        this->ctx.device
            .updateDescriptorSets(vk::WriteDescriptorSet{this->descriptor_sets[i].get(),
                                                         0,
                                                         0,
                                                         vk::DescriptorType::eCombinedImageSampler,
                                                         image_info,
                                                         nullptr,
                                                         nullptr},
                                  nullptr);
    }
}

std::vector<vk::UniqueFramebuffer> circuit_board::create_framebuffers(
    const std::vector<vk::UniqueImageView> &image_views) const {
    std::vector<vk::UniqueFramebuffer> framebuffers;
    for (int i = 0; i < this->image_count; ++i) {
        framebuffers.push_back(
            std::move(this->ctx.device.createFramebufferUnique({vk::FramebufferCreateFlags(),
                                                                this->render_pass,
                                                                image_views[i].get(),
                                                                this->width,
                                                                this->height,
                                                                1})));
    }
    return framebuffers;
}

bool circuit_board::resize() {
    if (this->target_width == this->width && this->target_height == this->height)
        return false;
    this->width = this->target_width;
    this->height = this->target_height;
    std::vector<vk::UniqueImage> new_images = std::move(this->create_images());
    vk::UniqueDeviceMemory new_memory = this->allocate_image_memory(new_images);
    std::vector<vk::UniqueImageView> new_image_views = std::move(
        this->create_image_views(new_images));
    this->update_descriptor_sets(new_image_views);
    std::vector<vk::UniqueFramebuffer> new_framebuffers = std::move(
        this->create_framebuffers(new_image_views));
    for (int i = 0; i < this->image_count; ++i) {
        this->command_buffers[i]->reset();
    }
    this->framebuffers = std::move(new_framebuffers);
    this->image_views = std::move(new_image_views);
    this->images = std::move(new_images);
    this->image_memory = std::move(new_memory);

    for (int i = 0; i < this->image_count; ++i) {
        this->record_command_buffer(i);
    }
    return true;
}
void circuit_board::set_target_size(const uint32_t width, const uint32_t height) {
    this->target_width = width;
    this->target_height = height;
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
           *this->images[image_index],
           vk::ImageSubresourceRange{vk::ImageAspectFlagBits::eColor, 0, 1, 0, 1}};

    command_buffer.pipelineBarrier(vk::PipelineStageFlagBits::eTopOfPipe,
                                   vk::PipelineStageFlagBits::eColorAttachmentOutput,
                                   {},
                                   nullptr,
                                   nullptr,
                                   barrier);

    std::vector clearValues{vk::ClearValue{vk::ClearColorValue{0.0f, 0.0f, 0.0f, 1.0f}}};
    const vk::RenderPassBeginInfo renderPassInfo(this->render_pass,
                                                 *this->framebuffers[image_index],
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

    command_buffer.endRenderPass();
    command_buffer.end();
}