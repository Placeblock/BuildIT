//
// Created by felix on 9/29/25.
//

#ifndef INSTANCED_INDIRECT_PIPELINE_H
#define INSTANCED_INDIRECT_PIPELINE_H
#include "builder/compute_pipeline_builder.h"

class instanced_indirect_pipeline {
public:
    instanced_indirect_pipeline(const vk::Device& device, const vk::RenderPass& render_pass);

    vk::UniquePipeline pipeline;
};

#endif //INSTANCED_INDIRECT_PIPELINE_H
