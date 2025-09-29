//
// Created by felix on 29.09.25.
//

#include "graphics_pipeline_builder.h"

#include <ranges>

graphics_pipeline_builder::graphics_pipeline_builder(const vk::Device &device,
                                                     const vk::RenderPass &render_pass)
    : device(device), render_pass(render_pass) {}

graphics_pipeline_builder &graphics_pipeline_builder::add_shader_stage(
    vk::ShaderStageFlagBits stage, vk::UniqueShaderModule module) {
    this->shader_stages.emplace_back(vk::PipelineShaderStageCreateFlags(), stage, *module, "main");
    return *this;
}
graphics_pipeline_builder &graphics_pipeline_builder::set_vertex_input_state(
    const vk::PipelineVertexInputStateCreateInfo &vertex_input_state) {
    this->vertex_input_state = vertex_input_state;
    return *this;
}
graphics_pipeline_builder &graphics_pipeline_builder::set_input_assembly_state(
    const vk::PipelineInputAssemblyStateCreateInfo &input_assembly_state) {
    this->input_assembly_state = input_assembly_state;
    return *this;
}
graphics_pipeline_builder &graphics_pipeline_builder::set_dynamic_state(
    const vk::PipelineDynamicStateCreateInfo &dynamic_state) {
    this->dynamic_state = dynamic_state;
    return *this;
}
graphics_pipeline_builder &graphics_pipeline_builder::set_viewport_state(
    const vk::PipelineViewportStateCreateInfo &viewport_state) {
    this->viewport_state = viewport_state;
    return *this;
}
graphics_pipeline_builder &graphics_pipeline_builder::set_rasterization_state(
    const vk::PipelineRasterizationStateCreateInfo &rasterization_state) {
    this->rasterization_state = rasterization_state;
    return *this;
}
graphics_pipeline_builder &graphics_pipeline_builder::set_multisample_state(
    const vk::PipelineMultisampleStateCreateInfo &multisample_state) {
    this->multisample_state = multisample_state;
    return *this;
}
graphics_pipeline_builder &graphics_pipeline_builder::set_color_blend_state(
    const vk::PipelineColorBlendStateCreateInfo &color_blend_state) {
    this->color_blend_state = color_blend_state;
    return *this;
}

graphics_pipeline_builder &graphics_pipeline_builder::add_descriptor_set_layout(
    vk::UniqueDescriptorSetLayout layout) {
    this->descriptor_set_layouts.emplace_back(std::move(layout));
    return *this;
}
graphics_pipeline_builder &graphics_pipeline_builder::add_push_constant_range(
    const vk::PushConstantRange &layout) {
    this->push_constant_ranges.emplace_back(layout);
    return *this;
}

vk::UniquePipeline graphics_pipeline_builder::build() {
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
    vk::GraphicsPipelineCreateInfo pipeline_create_info = {vk::PipelineCreateFlags(),
                                                           this->shader_stages,
                                                           &this->vertex_input_state,
                                                           &this->input_assembly_state,
                                                           {},
                                                           &this->viewport_state,
                                                           &this->rasterization_state,
                                                           &this->multisample_state,
                                                           nullptr,
                                                           &this->color_blend_state,
                                                           &this->dynamic_state,
                                                           *layout,
                                                           *this->render_pass,
                                                           0,
                                                           {},
                                                           -1};
    auto pipeline = this->device.createGraphicsPipelineUnique(nullptr, pipeline_create_info);
    if (pipeline.result != vk::Result::eSuccess) {
        throw std::runtime_error("failed to create compute pipeline");
    }
    return std::move(pipeline.value);
}