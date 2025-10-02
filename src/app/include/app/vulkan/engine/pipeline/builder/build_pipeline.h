//
// Created by felix on 02.10.25.
//

#ifndef PIPELINE_H
#define PIPELINE_H
#include <vulkan/vulkan.hpp>

struct build_pipeline {
    vk::UniquePipeline pipeline;
    vk::UniquePipelineLayout layout;
};

#endif //PIPELINE_H
