//
// Created by felix on 18.07.25.
//

#ifndef CIRCUITBOARD_MANAGER_H
#define CIRCUITBOARD_MANAGER_H
#include "app/vulkan/vulkancontext.hpp"
#include "circuitboard.hpp"
#include <memory>
#include <vulkan/vulkan.hpp>

class circuitboard_manager {
public:
    vk::UniqueSampler sampler;

    circuitboard_manager(const vulkan_context& ctx);

    circuit_board* create_board();

    ~circuitboard_manager() {
        std::cout << "destroying circuitboard manager" << std::endl;
    }

private:
    vk::ShaderModule createShaderModule(const std::string& code) const;

    void create_descriptor_pool();
    void create_sampler();
    void create_command_pool();
    void create_descriptor_set_layout();
    void create_render_pass();
    void create_pipeline();

    vk::UniqueRenderPass render_pass;
    vk::UniqueDescriptorPool descriptor_pool;
    vk::UniqueDescriptorSetLayout descriptor_set_layout;
    vk::UniqueCommandPool command_pool;
    vk::UniquePipelineLayout pipeline_layout;
    vk::UniquePipeline pipeline;

    const vulkan_context& ctx;

    std::vector<std::unique_ptr<circuit_board>> circuit_boards;
};

#endif //CIRCUITBOARD_MANAGER_H
