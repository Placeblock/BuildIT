//
// Created by felix on 15.07.25.
//
#include "app/vulkan/vulkancontext.h"
#include <vulkan/vulkan.hpp>

class circuit_board {
public:
    circuit_board(const vulkan_context ctx,
                  const vk::DescriptorPool descriptor_pool,
                  const vk::DescriptorSetLayout descriptor_set_layout,
                  const vk::Sampler sampler,
                  const uint32_t width,
                  const uint32_t height) {
        // Create Image the circuit board is rendered onto
        std::vector queue_family_indices = {ctx.queue_family_indices.graphics_family};
        this->image = ctx.device.createImage(
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
        // Create Image View
        this->image_view = ctx.device.createImageView(vk::ImageViewCreateInfo{
            vk::ImageViewCreateFlags(),
            this->image,
            vk::ImageViewType::e2D,
            vk::Format::eB8G8R8A8Srgb,
            vk::ComponentMapping{vk::ComponentSwizzle::eIdentity,
                                 vk::ComponentSwizzle::eIdentity,
                                 vk::ComponentSwizzle::eIdentity,
                                 vk::ComponentSwizzle::eIdentity},
            vk::ImageSubresourceRange{vk::ImageAspectFlagBits::eColor, 0, 1, 0, 1}});
        // Allocate the descriptor set for the circuit board
        const std::vector<vk::DescriptorSet> descriptor_sets = ctx.device.allocateDescriptorSets(
            vk::DescriptorSetAllocateInfo{descriptor_pool, 1, &descriptor_set_layout});
        this->descriptor_set = descriptor_sets[0];
        vk::DescriptorImageInfo image_info{sampler,
                                           this->image_view,
                                           vk::ImageLayout::eShaderReadOnlyOptimal};
        ctx.device
            .updateDescriptorSets(vk::WriteDescriptorSet{this->descriptor_set,
                                                         0,
                                                         0,

                                                         vk::DescriptorType::eCombinedImageSampler,
                                                         image_info,
                                                         nullptr,
                                                         nullptr},
                                  nullptr);
    }

    void resize(const int width, const int height) {}

private:
    vk::Image image;
    vk::ImageView image_view;
    vk::DescriptorSet descriptor_set;
};