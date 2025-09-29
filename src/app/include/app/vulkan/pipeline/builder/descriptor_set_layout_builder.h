//
// Created by felix on 9/29/25.
//

#ifndef DESCRIPTOR_SET_LAYOUT_BUILDER_H
#define DESCRIPTOR_SET_LAYOUT_BUILDER_H
#include <vector>
#include <vulkan/vulkan.hpp>

class descriptor_set_layout_builder {
    const vk::Device& device;

    std::vector<vk::DescriptorSetLayoutBinding> bindings;

public:
    explicit descriptor_set_layout_builder(const vk::Device& device);

    descriptor_set_layout_builder& add_binding(uint index,
                                               vk::DescriptorType descriptor_type,
                                               uint descriptor_count,
                                               vk::ShaderStageFlagBits stages);

    vk::UniqueDescriptorSetLayout build();
};

#endif //DESCRIPTOR_SET_LAYOUT_BUILDER_H
