//
// Created by felix on 02.10.25.
//

#ifndef PIPELINE_H
#define PIPELINE_H
#include <vulkan/vulkan.hpp>

namespace bit {
struct pipeline {
    vk::UniquePipeline handle;
    vk::UniquePipelineLayout layout;
};
}

#endif //PIPELINE_H
