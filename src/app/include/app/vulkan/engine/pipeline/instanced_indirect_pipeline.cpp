//
// Created by felix on 9/29/25.
//

#include "instanced_indirect_pipeline.h"

#include "app/vulkan/shader.hpp"
#include "builder/descriptor_set_layout_builder.h"
#include "builder/graphics_pipeline_builder.h"
#include "spdlog/spdlog.h"

instanced_indirect_pipeline::instanced_indirect_pipeline(const vk::Device &device,
                                                         const vk::RenderPass &render_pass) {
    spdlog::debug("Creating instanced indirect pipeline");
    auto dynamic_states = std::vector{vk::DynamicState::eViewport, vk::DynamicState::eScissor};
    vk::Viewport viewport{};
    vk::Rect2D scissor{};
    vk::PipelineColorBlendAttachmentState colorBlendAttachment(false,
                                                               vk::BlendFactor::eOne,
                                                               vk::BlendFactor::eZero,
                                                               vk::BlendOp::eAdd,
                                                               vk::BlendFactor::eOne,
                                                               vk::BlendFactor::eZero,
                                                               vk::BlendOp::eAdd,
                                                               vk::ColorComponentFlagBits::eR
                                                               | vk::ColorComponentFlagBits::eG
                                                               | vk::ColorComponentFlagBits::eB
                                                               | vk::ColorComponentFlagBits::eA);
    const vk::PipelineColorBlendStateCreateInfo
        colorBlending(vk::PipelineColorBlendStateCreateFlags(),
                      false,
                      vk::LogicOp::eCopy,
                      colorBlendAttachment,
                      {0.0, 0.0, 0.0, 0.0});
    this->descriptor_set_layout = descriptor_set_layout_builder(device)
        .add_binding(0,
                     vk::DescriptorType::eStorageBuffer,
                     1,
                     vk::ShaderStageFlagBits::eVertex)
        .build();
    vk::UniqueShaderModule vertShader = loadShaderUnique(device, "instanced-rect.vert.spv");
    vk::UniqueShaderModule fragShader = loadShaderUnique(device, "instanced-rect.frag.spv");
    this->pipeline
        = graphics_pipeline_builder(device, render_pass)
        .add_shader_stage(vk::ShaderStageFlagBits::eVertex, *vertShader)
        .add_shader_stage(vk::ShaderStageFlagBits::eFragment, *fragShader)
        .add_descriptor_set_layout(*this->descriptor_set_layout)
        .add_push_constant_range(
            vk::PushConstantRange{vk::ShaderStageFlagBits::eVertex, 0, 3 * 3 * sizeof(float)})
        .set_vertex_input_state({vk::PipelineVertexInputStateCreateFlags(), nullptr, nullptr})
        .set_input_assembly_state({vk::PipelineInputAssemblyStateCreateFlags(),
                                   vk::PrimitiveTopology::eTriangleList,
                                   false})
        .set_dynamic_state({vk::PipelineDynamicStateCreateFlags(), dynamic_states})
        .set_viewport_state({vk::PipelineViewportStateCreateFlags(), viewport, scissor})
        .set_rasterization_state({vk::PipelineRasterizationStateCreateFlags(),
                                  false,
                                  false,
                                  vk::PolygonMode::eFill,
                                  vk::CullModeFlagBits::eBack,
                                  vk::FrontFace::eClockwise,
                                  false,
                                  0.0f,
                                  0.0f,
                                  0.0f,
                                  1.0f})
        .set_color_blend_state(colorBlending)
        .build();
}