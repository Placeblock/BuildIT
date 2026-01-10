//
// Created by felix on 1/4/26.
//

#include "bit/pipeline/descriptor_set_layout_builder.hpp"

descriptor_set_layout_builder_t::descriptor_set_layout_builder_t(const vk::Device &device)
    : device(device) {
}

descriptor_set_layout_builder_t &descriptor_set_layout_builder_t::add_binding(
    uint index,
    vk::DescriptorType descriptor_type,
    uint descriptor_count,
    vk::ShaderStageFlags stages) {
    this->bindings.emplace_back(index, descriptor_type, descriptor_count, stages);
    return *this;
}

vk::UniqueDescriptorSetLayout descriptor_set_layout_builder_t::build() const {
    return this->device.createDescriptorSetLayoutUnique(
        vk::DescriptorSetLayoutCreateInfo(vk::DescriptorSetLayoutCreateFlags(), this->bindings));
}