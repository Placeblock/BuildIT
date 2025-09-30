//
// Created by felix on 29.09.25.
//

#ifndef GRAPHICS_PIPELINE_BUILDER_H
#define GRAPHICS_PIPELINE_BUILDER_H
#include <vulkan/vulkan.hpp>

class graphics_pipeline_builder {
public:
    graphics_pipeline_builder(const vk::Device& device, const vk::RenderPass& render_pass);

    graphics_pipeline_builder& add_shader_stage(vk::ShaderStageFlagBits stage,
                                                vk::UniqueShaderModule module);

    graphics_pipeline_builder& set_vertex_input_state(
        const vk::PipelineVertexInputStateCreateInfo& vertex_input_state);
    graphics_pipeline_builder& set_input_assembly_state(
        const vk::PipelineInputAssemblyStateCreateInfo& input_assembly_state);
    graphics_pipeline_builder& set_dynamic_state(
        const vk::PipelineDynamicStateCreateInfo& dynamic_state);
    graphics_pipeline_builder& set_viewport_state(
        const vk::PipelineViewportStateCreateInfo& viewport_state);
    graphics_pipeline_builder& set_rasterization_state(
        const vk::PipelineRasterizationStateCreateInfo& rasterization_state);
    graphics_pipeline_builder& set_multisample_state(
        const vk::PipelineMultisampleStateCreateInfo& multisample_state);
    graphics_pipeline_builder& set_color_blend_state(
        const vk::PipelineColorBlendStateCreateInfo& color_blend_state);

    graphics_pipeline_builder& add_descriptor_set_layout(const vk::DescriptorSetLayout& layout);
    graphics_pipeline_builder& add_push_constant_range(const vk::PushConstantRange& layout);

    vk::UniquePipeline build();

private:
    std::vector<const vk::DescriptorSetLayout> descriptor_set_layouts;
    std::vector<const vk::PushConstantRange> push_constant_ranges;

    std::vector<vk::PipelineShaderStageCreateInfo> shader_stages;

    vk::PipelineVertexInputStateCreateInfo vertex_input_state;
    vk::PipelineInputAssemblyStateCreateInfo input_assembly_state;
    vk::PipelineDynamicStateCreateInfo dynamic_state;
    vk::PipelineViewportStateCreateInfo viewport_state;
    vk::PipelineRasterizationStateCreateInfo rasterization_state;
    vk::PipelineMultisampleStateCreateInfo multisample_state;
    vk::PipelineColorBlendStateCreateInfo color_blend_state;

    const vk::Device& device;

    const vk::RenderPass& render_pass;
};

#endif //GRAPHICS_PIPELINE_BUILDER_H
