//
// Created by felix on 9/29/25.
//

#include "descriptor_set_layout_builder.h"

descriptor_set_layout_builder::descriptor_set_layout_builder(const vk::Device &device)
    : device(device) {}

descriptor_set_layout_builder &descriptor_set_layout_builder::add_binding(
    uint index,
    vk::DescriptorType descriptor_type,
    uint descriptor_count,
    vk::ShaderStageFlagBits stages) {
    this->bindings.emplace_back(index, descriptor_type, descriptor_count, stages);
    return *this;
}

vk::UniqueDescriptorSetLayout descriptor_set_layout_builder::build() {
    return this->device.createDescriptorSetLayoutUnique(
        vk::DescriptorSetLayoutCreateInfo(vk::DescriptorSetLayoutCreateFlags(), this->bindings));
}