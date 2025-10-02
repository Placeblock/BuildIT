//
// Created by felix on 9/29/25.
//

#include "compute_pipeline_builder.h"

#include <ranges>

compute_pipeline_builder::compute_pipeline_builder(const vk::Device &device) : device(device) {}

compute_pipeline_builder &compute_pipeline_builder::add_descriptor_set_layout(
    const vk::DescriptorSetLayout &layout) {
    this->descriptor_set_layouts.emplace_back(layout);
    return *this;
}
compute_pipeline_builder &compute_pipeline_builder::add_push_constant_range(
    const vk::PushConstantRange &layout) {
    this->push_constant_ranges.emplace_back(layout);
    return *this;
}
compute_pipeline_builder &compute_pipeline_builder::set_shader_module(
    vk::UniqueShaderModule *module) {
    this->module = std::move(*module);
    return *this;
}

build_pipeline compute_pipeline_builder::build() {
    vk::UniquePipelineLayout layout = this->device.createPipelineLayoutUnique(
        vk::PipelineLayoutCreateInfo(vk::PipelineLayoutCreateFlags(),
                                     this->descriptor_set_layouts,
                                     this->push_constant_ranges));
    auto pipe = this->device.createComputePipelineUnique(
        nullptr,
        vk::ComputePipelineCreateInfo(
            vk::PipelineCreateFlags(),
            vk::PipelineShaderStageCreateInfo(vk::PipelineShaderStageCreateFlags(),
                                              vk::ShaderStageFlagBits::eCompute,
                                              *this->module,
                                              "main"),
            *layout));
    if (pipe.result != vk::Result::eSuccess) {
        throw std::runtime_error("failed to create compute pipeline");
    }
    build_pipeline result(std::move(pipe.value), std::move(layout));
    return result;
}