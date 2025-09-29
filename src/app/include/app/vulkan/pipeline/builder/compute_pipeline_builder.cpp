//
// Created by felix on 9/29/25.
//

#include "compute_pipeline_builder.h"

#include <ranges>

compute_pipeline_builder::compute_pipeline_builder(const vk::Device &device) : device(device) {}

compute_pipeline_builder &compute_pipeline_builder::add_descriptor_set_layout(
    vk::UniqueDescriptorSetLayout layout) {
    this->descriptor_set_layouts.emplace_back(std::move(layout));
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

vk::UniquePipeline compute_pipeline_builder::build() {
    auto descriptor_set_layouts_view = this->descriptor_set_layouts
                                       | std::ranges::views::transform(
                                           [](const vk::UniqueDescriptorSetLayout &x) {
                                               return *x;
                                           });
    std::vector descriptor_set_layouts(descriptor_set_layouts_view.begin(),
                                       descriptor_set_layouts_view.end());

    vk::UniquePipelineLayout layout = this->device.createPipelineLayoutUnique(
        vk::PipelineLayoutCreateInfo(vk::PipelineLayoutCreateFlags(),
                                     descriptor_set_layouts,
                                     this->push_constant_ranges));
    auto pipeline = this->device.createComputePipelineUnique(
        nullptr,
        vk::ComputePipelineCreateInfo(
            vk::PipelineCreateFlags(),
            vk::PipelineShaderStageCreateInfo(vk::PipelineShaderStageCreateFlags(),
                                              vk::ShaderStageFlagBits::eCompute,
                                              *this->module,
                                              "main"),
            *layout));
    if (pipeline.result != vk::Result::eSuccess) {
        throw std::runtime_error("failed to create compute pipeline");
    }
    return std::move(pipeline.value);
}