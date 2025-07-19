//
// Created by felix on 15.07.25.
//

#include "app/vulkan/circuitboard.hpp"

#include <iostream>

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
    , render_pass(render_pass)
    , pipeline(pipeline)
    , sampler(sampler)
    , descriptor_pool(descriptor_pool)
    , graphics_family_index(ctx.queue_families.graphics_family) {
    // Create Images the circuit board is rendered onto
    this->create_images();
    // Create Memory for the Images and bind them to the memory
    this->allocate_image_memory();
    // Create Image Views for the Images
    this->create_image_views();
    // Allocate the descriptor set for the circuit board
    std::vector layouts = {descriptor_set_layout, descriptor_set_layout, descriptor_set_layout};
    this->descriptor_sets = ctx.device.allocateDescriptorSetsUnique(
        vk::DescriptorSetAllocateInfo{descriptor_pool, layouts});
    // Bind image views to the descriptor sets
    this->update_descriptor_sets();
    this->create_framebuffers();

    this->command_buffers = this->ctx.device.allocateCommandBuffersUnique(
        vk::CommandBufferAllocateInfo{command_pool, vk::CommandBufferLevel::ePrimary, image_count});
    for (int i = 0; i < this->image_count; ++i) {
        this->record_command_buffer(i);
    }

    this->render_finished_semaphore = this->ctx.device.createSemaphoreUnique(
        vk::SemaphoreCreateInfo{});
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

void circuit_board::create_images() {
    std::vector queue_family_indices = {this->graphics_family_index};
    for (int i = 0; i < this->image_count; ++i) {
        this->images.push_back(this->ctx.device.createImageUnique(
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
                                vk::ImageLayout::eUndefined}));
    }
}

void circuit_board::allocate_image_memory() {
    vk::DeviceSize total_size = 0;
    vk::DeviceSize offsets[this->image_count];
    uint32_t image_memory_type;
    for (int i = 0; i < this->image_count; ++i) {
        const vk::MemoryRequirements memory_requirements
            = this->ctx.device.getImageMemoryRequirements(this->images[i].get());
        offsets[i] = (total_size + memory_requirements.alignment - 1)
                     & ~(memory_requirements.alignment - 1);
        total_size = offsets[i] + memory_requirements.size;
        if (i == 0) {
            image_memory_type = this->find_memory_type(memory_requirements.memoryTypeBits,
                                                       vk::MemoryPropertyFlagBits::eDeviceLocal);
        }
    }
    const vk::MemoryAllocateInfo memory_allocate_info{total_size, image_memory_type};
    this->image_memory = this->ctx.device.allocateMemoryUnique(memory_allocate_info);
    for (int i = 0; i < this->image_count; ++i) {
        this->ctx.device.bindImageMemory(this->images[i].get(),
                                         this->image_memory.get(),
                                         offsets[i]);
    }
}

void circuit_board::create_image_views() {
    // Create Image Views
    for (int i = 0; i < this->image_count; ++i) {
        this->image_views.push_back(this->ctx.device.createImageViewUnique(vk::ImageViewCreateInfo{
            vk::ImageViewCreateFlags(),
            this->images[i].get(),
            vk::ImageViewType::e2D,
            vk::Format::eR8G8B8A8Unorm,
            vk::ComponentMapping{vk::ComponentSwizzle::eIdentity,
                                 vk::ComponentSwizzle::eIdentity,
                                 vk::ComponentSwizzle::eIdentity,
                                 vk::ComponentSwizzle::eIdentity},
            vk::ImageSubresourceRange{vk::ImageAspectFlagBits::eColor, 0, 1, 0, 1}}));
    }
}

void circuit_board::update_descriptor_sets() const {
    for (int i = 0; i < this->image_count; ++i) {
        vk::DescriptorImageInfo image_info{this->sampler,
                                           this->image_views[i].get(),
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

void circuit_board::create_framebuffers() {
    for (int i = 0; i < this->image_count; ++i) {
        this->framebuffers.push_back(
            this->ctx.device.createFramebufferUnique({vk::FramebufferCreateFlags(),
                                                      this->render_pass,
                                                      this->image_views[i].get(),
                                                      this->width,
                                                      this->height,
                                                      1}));
    }
}

void circuit_board::resize(const int width, const int height) {
    if (width == this->width && height == this->height)
        return;
    this->width = width;
    this->height = height;

    this->framebuffers.clear();
    this->image_views.clear();
    this->images.clear();
    this->image_memory.reset();
    this->create_images();
    this->allocate_image_memory();
    this->create_image_views();
    this->update_descriptor_sets();
    this->create_framebuffers();
}

void circuit_board::record_command_buffer(const uint32_t image_index) {
    std::cout << "RECORDING COMMAND BUFFER\n";
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

void circuit_board::render(const vk::Queue &queue, const uint32_t image_index) {
    constexpr std::vector<vk::Semaphore> wait_semaphores;
    constexpr std::vector<vk::PipelineStageFlags> waitStages;
    const vk::SubmitInfo submitInfo{wait_semaphores,
                                    waitStages,
                                    *this->command_buffers[image_index],
                                    *this->render_finished_semaphore};
    queue.submit(submitInfo, nullptr);
}