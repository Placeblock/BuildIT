//
// Created by felix on 9/29/25.
//

#include "reset_culling_pipeline.h"

#include "app/vulkan/shader.hpp"
#include "builder/compute_pipeline_builder.h"
#include "builder/descriptor_set_layout_builder.h"
#include "spdlog/spdlog.h"

reset_culling_pipeline::reset_culling_pipeline(const vk::Device &device) {
    spdlog::debug("Creating reset culling pipeline");
    auto module = loadShaderUnique(device, "reset-instance-count.comp.spv");

    this->descriptor_set_layout = descriptor_set_layout_builder(device)
        .add_binding(0,
                     vk::DescriptorType::eStorageBuffer,
                     1,
                     vk::ShaderStageFlagBits::eCompute)
        .build();

    this->pipeline = compute_pipeline_builder(device)
        .add_descriptor_set_layout(*this->descriptor_set_layout)
        .set_shader_module(&module)
        .build();
}