//
// Created by felix on 9/29/25.
//

#include "culling_pipeline.h"

#include "app/vulkan/shader.hpp"
#include "builder/descriptor_set_layout_builder.h"
#include "spdlog/spdlog.h"

culling_pipeline::culling_pipeline(const vk::Device &device) {
    spdlog::debug("Creating culling pipeline");
    auto module = loadShaderUnique(device, "rect-culling.comp.spv");
    this->descriptor_set_layout = descriptor_set_layout_builder(device)
        .add_binding(0,
                     vk::DescriptorType::eStorageBuffer,
                     1,
                     vk::ShaderStageFlagBits::eCompute)
        .add_binding(1,
                     vk::DescriptorType::eStorageBuffer,
                     1,
                     vk::ShaderStageFlagBits::eCompute)
        .add_binding(2,
                     vk::DescriptorType::eStorageBuffer,
                     1,
                     vk::ShaderStageFlagBits::eCompute)
        .build();
    this->pipeline = compute_pipeline_builder(device)
        .add_descriptor_set_layout(*this->descriptor_set_layout)
        .add_push_constant_range(
            {vk::ShaderStageFlagBits::eCompute, 0, 4 * sizeof(float)})
        .set_shader_module(&module)
        .build();
}