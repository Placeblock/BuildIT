//
// Created by felix on 9/29/25.
//

#ifndef RESET_CULLING_PIPELINE_H
#define RESET_CULLING_PIPELINE_H
#include "builder/build_pipeline.h"
#include <vulkan/vulkan.hpp>

class reset_culling_pipeline {
public:
    explicit reset_culling_pipeline(const vk::Device& device);

    vk::UniqueDescriptorSetLayout descriptor_set_layout;
    build_pipeline pipeline;
};

#endif //RESET_CULLING_PIPELINE_H
