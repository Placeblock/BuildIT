//
// Created by felix on 9/29/25.
//

#ifndef CULLING_PIPELINE_H
#define CULLING_PIPELINE_H
#include "builder/compute_pipeline_builder.h"

class culling_pipeline {
public:
    explicit culling_pipeline(const vk::Device& device);

    vk::UniqueDescriptorSetLayout descriptor_set_layout;
    vk::UniquePipeline pipeline;
};

#endif //CULLING_PIPELINE_H
