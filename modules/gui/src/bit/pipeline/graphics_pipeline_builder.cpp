//
// Created by felix on 29.09.25.
//

#include "bit/pipeline/graphics_pipeline_builder.hpp"

#include <ranges>

using namespace bit;

graphics_pipeline_builder::graphics_pipeline_builder(const vk::Device &device,
                                                     const vk::RenderPass &render_pass)
    : device(device), render_pass(render_pass) {
}

graphics_pipeline_builder &graphics_pipeline_builder::add_shader_stage(
    vk::ShaderStageFlagBits stage,
    vk::ShaderModule &module) {
    this->shader_stages.emplace_back(vk::PipelineShaderStageCreateFlags(), stage, module, "main");
    return *this;
}

graphics_pipeline_builder &graphics_pipeline_builder::set_vertex_input_state(
    const vk::PipelineVertexInputStateCreateInfo &new_vertex_input_state) {
    this->vertex_input_state = new_vertex_input_state;
    return *this;
}

graphics_pipeline_builder &graphics_pipeline_builder::set_input_assembly_state(
    const vk::PipelineInputAssemblyStateCreateInfo &new_input_assembly_state) {
    this->input_assembly_state = new_input_assembly_state;
    return *this;
}

graphics_pipeline_builder &graphics_pipeline_builder::set_dynamic_state(
    const vk::PipelineDynamicStateCreateInfo &new_dynamic_state) {
    this->dynamic_state = new_dynamic_state;
    return *this;
}

graphics_pipeline_builder &graphics_pipeline_builder::set_viewport_state(
    const vk::PipelineViewportStateCreateInfo &new_viewport_state) {
    this->viewport_state = new_viewport_state;
    return *this;
}

graphics_pipeline_builder &graphics_pipeline_builder::set_rasterization_state(
    const vk::PipelineRasterizationStateCreateInfo &new_rasterization_state) {
    this->rasterization_state = new_rasterization_state;
    return *this;
}

graphics_pipeline_builder &graphics_pipeline_builder::set_multisample_state(
    const vk::PipelineMultisampleStateCreateInfo &new_multisample_state) {
    this->multisample_state = new_multisample_state;
    return *this;
}

graphics_pipeline_builder &graphics_pipeline_builder::set_color_blend_state(
    const vk::PipelineColorBlendStateCreateInfo &new_color_blend_state) {
    this->color_blend_state = new_color_blend_state;
    return *this;
}

graphics_pipeline_builder &graphics_pipeline_builder::add_descriptor_set_layout(
    const vk::DescriptorSetLayout &layout) {
    this->descriptor_set_layouts.emplace_back(layout);
    return *this;
}

graphics_pipeline_builder &graphics_pipeline_builder::add_push_constant_range(
    const vk::PushConstantRange &layout) {
    this->push_constant_ranges.emplace_back(layout);
    return *this;
}

pipeline graphics_pipeline_builder::build() {
    vk::UniquePipelineLayout layout = this->device.createPipelineLayoutUnique(
        vk::PipelineLayoutCreateInfo(vk::PipelineLayoutCreateFlags(),
                                     this->descriptor_set_layouts,
                                     this->push_constant_ranges));
    const vk::GraphicsPipelineCreateInfo pipeline_create_info = {vk::PipelineCreateFlags(),
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
                                                                 this->render_pass,
                                                                 0,
                                                                 {},
                                                                 -1};
    auto pipe = this->device.createGraphicsPipelineUnique(nullptr, pipeline_create_info);
    if (pipe.result != vk::Result::eSuccess) {
        throw std::runtime_error("failed to create compute pipeline");
    }
    pipeline result(std::move(pipe.value), std::move(layout));
    return result;
}