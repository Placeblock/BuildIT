//
// Created by felix on 9/29/25.
//

#ifndef COMPUTE_PIPELINE_H
#define COMPUTE_PIPELINE_H
#include "bit/pipeline/build_pipeline.hpp"
#include <vulkan/vulkan.hpp>

namespace bit {
class compute_pipeline_builder {
    const vk::Device &device;

    std::vector<vk::DescriptorSetLayout> descriptor_set_layouts;
    std::vector<vk::PushConstantRange> push_constant_ranges;
    vk::UniqueShaderModule module;

public:
    explicit compute_pipeline_builder(const vk::Device &device);

    compute_pipeline_builder &add_descriptor_set_layout(const vk::DescriptorSetLayout &layout);

    compute_pipeline_builder &add_push_constant_range(const vk::PushConstantRange &layout);

    compute_pipeline_builder &set_shader_module(vk::UniqueShaderModule &new_module);

    pipeline build();
};
}

#endif //COMPUTE_PIPELINE_H
