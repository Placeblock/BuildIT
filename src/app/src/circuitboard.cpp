//
// Created by felix on 15.07.25.
//
#include "app/vulkan/vulkancontext.h"
#include <vulkan/vulkan.hpp>

class circuit_board {
public:
    circuit_board(const vulkan_context &ctx,
                  const vk::DescriptorPool &descriptor_pool,
                  const vk::DescriptorSetLayout &descriptor_set_layout,
                  const vk::Sampler sampler,
                  const uint32_t image_memory_type,
                  const uint32_t width,
                  const uint32_t height)
        : device(ctx.device)
        , sampler(sampler)
        , descriptor_pool(descriptor_pool)
        , graphics_family_index(ctx.queue_families.graphics_family)
        , image_memory_type(image_memory_type) {
        // Create Image the circuit board is rendered onto
        this->create_image(width, height);
        this->allocate_image_memory();
        this->create_image_view();
        // Allocate the descriptor set for the circuit board
        std::vector<vk::UniqueDescriptorSet> descriptor_sets
            = ctx.device.allocateDescriptorSetsUnique(
                vk::DescriptorSetAllocateInfo{descriptor_pool, 1, &descriptor_set_layout});
        this->descriptor_set = std::move(descriptor_sets[0]);
        this->update_descriptor_set();
    }

    void create_image(const uint32_t width, const uint32_t height) {
        std::vector queue_family_indices = {this->graphics_family_index};
        this->image = this->device.createImageUnique(
            vk::ImageCreateInfo{vk::ImageCreateFlags(),
                                vk::ImageType::e2D,
                                vk::Format::eB8G8R8A8Srgb,
                                {width, height},
                                1,
                                1,
                                vk::SampleCountFlagBits::e1,
                                vk::ImageTiling::eOptimal,
                                vk::ImageUsageFlagBits::eColorAttachment
                                    | vk::ImageUsageFlagBits::eSampled,
                                vk::SharingMode::eExclusive,
                                queue_family_indices,
                                vk::ImageLayout::eUndefined});
    }

    void allocate_image_memory() {
        const vk::MemoryRequirements memory_requirements = this->device.getImageMemoryRequirements(
            this->image.get());
        const vk::MemoryAllocateInfo memory_allocate_info{memory_requirements.size,
                                                          image_memory_type};
        this->image_memory = this->device.allocateMemoryUnique(memory_allocate_info);
        this->device.bindImageMemory(this->image.get(), this->image_memory.get(), 0);
    }

    void create_image_view() {
        // Create Image View
        this->image_view = this->device.createImageViewUnique(vk::ImageViewCreateInfo{
            vk::ImageViewCreateFlags(),
            this->image.get(),
            vk::ImageViewType::e2D,
            vk::Format::eB8G8R8A8Srgb,
            vk::ComponentMapping{vk::ComponentSwizzle::eIdentity,
                                 vk::ComponentSwizzle::eIdentity,
                                 vk::ComponentSwizzle::eIdentity,
                                 vk::ComponentSwizzle::eIdentity},
            vk::ImageSubresourceRange{vk::ImageAspectFlagBits::eColor, 0, 1, 0, 1}});
    }

    void update_descriptor_set() const {
        vk::DescriptorImageInfo image_info{this->sampler,
                                           this->image_view.get(),
                                           vk::ImageLayout::eShaderReadOnlyOptimal};
        this->device
            .updateDescriptorSets(vk::WriteDescriptorSet{this->descriptor_set.get(),
                                                         0,
                                                         0,
                                                         vk::DescriptorType::eCombinedImageSampler,
                                                         image_info,
                                                         nullptr,
                                                         nullptr},
                                  nullptr);
    }

    void resize(const int width, const int height) {
        this->image_view.reset();
        this->image.reset();
        this->image_memory.reset();
        this->create_image(width, height);
        this->allocate_image_memory();
        this->create_image_view();
        this->update_descriptor_set();
    }

    void render() {}

    vk::ImageView *get_image_view() {
        return &this->image_view.get();
    }

private:
    vk::UniqueImageView image_view;
    vk::UniqueImage image;
    vk::UniqueDeviceMemory image_memory;
    vk::UniqueDescriptorSet descriptor_set;

    // For RAII deconstructor and reallocation ond resize
    const vk::Device &device;
    const vk::Sampler &sampler;
    const vk::DescriptorPool &descriptor_pool;
    const uint32_t graphics_family_index;
    const uint32_t image_memory_type;
};