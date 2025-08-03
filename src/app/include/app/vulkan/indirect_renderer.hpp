//
// Created by felix on 8/3/25.
//

#ifndef INDIRECT_RENDERER_H
#define INDIRECT_RENDERER_H

#include "circuitboard.hpp"
#include "glm/common.hpp"
#include "shader.hpp"
#include "vulkancontext.hpp"
#include <ranges>
#include <vulkan/vulkan.hpp>

constexpr uint32_t BUFFER_SIZE = 1032;
constexpr uint32_t WORKGROUP_SIZE = 32;

struct instance {
    float x;
    float y;
    float width;
    float height;
};

class indirect_renderer {
public:
    vk::UniqueCommandBuffer computeCommandBuffer;
    std::vector<vk::UniqueCommandBuffer> drawCommandBuffers;

    explicit indirect_renderer(const vulkan_context& ctx) : ctx(ctx) {
        this->drawCommandPool = ctx.device.createCommandPoolUnique(
            vk::CommandPoolCreateInfo{vk::CommandPoolCreateFlagBits::eResetCommandBuffer,
                                      this->ctx.queue_families.graphics_family});
        this->computeCommandPool = ctx.device.createCommandPoolUnique(
            vk::CommandPoolCreateInfo{vk::CommandPoolCreateFlagBits::eResetCommandBuffer,
                                      this->ctx.queue_families.compute_family});

        std::vector instances_queue_families = {ctx.queue_families.compute_family};
        this->instancesBuffer = ctx.device.createBufferUnique(
            vk::BufferCreateInfo(vk::BufferCreateFlags(),
                                 BUFFER_SIZE * sizeof(instance),
                                 vk::BufferUsageFlagBits::eStorageBuffer,
                                 vk::SharingMode::eExclusive,
                                 instances_queue_families));

        std::vector vis_instances_queue_families = {ctx.queue_families.compute_family,
                                                    ctx.queue_families.graphics_family};
        this->visibleInstancesBuffer = ctx.device.createBufferUnique(
            vk::BufferCreateInfo(vk::BufferCreateFlags(),
                                 BUFFER_SIZE * sizeof(instance),
                                 vk::BufferUsageFlagBits::eStorageBuffer
                                     | vk::BufferUsageFlagBits::eVertexBuffer,
                                 vk::SharingMode::eExclusive,
                                 vis_instances_queue_families));

        std::vector draw_count_queue_families = {ctx.queue_families.compute_family};
        this->drawCountBuffer = ctx.device.createBufferUnique(
            vk::BufferCreateInfo(vk::BufferCreateFlags(),
                                 sizeof(uint32_t),
                                 vk::BufferUsageFlagBits::eStorageBuffer,
                                 vk::SharingMode::eExclusive,
                                 draw_count_queue_families));

        auto module = vk::UniqueShaderModule(loadShader(ctx, "rect-culling.comp"));

        std::array<vk::DescriptorSetLayoutBinding, 3> bindings{};
        bindings[0].binding = 0;
        bindings[0].stageFlags = vk::ShaderStageFlagBits::eCompute;
        bindings[0].descriptorCount = 1;
        bindings[0].descriptorType = vk::DescriptorType::eStorageBuffer;
        bindings[1].binding = 1;
        bindings[1].stageFlags = vk::ShaderStageFlagBits::eCompute;
        bindings[1].descriptorCount = 1;
        bindings[1].descriptorType = vk::DescriptorType::eStorageBuffer;
        bindings[2].binding = 2;
        bindings[2].stageFlags = vk::ShaderStageFlagBits::eCompute;
        bindings[2].descriptorCount = 1;
        bindings[2].descriptorType = vk::DescriptorType::eStorageBuffer;
        vk::UniqueDescriptorSetLayout computeLayout = ctx.device.createDescriptorSetLayoutUnique(
            vk::DescriptorSetLayoutCreateInfo(vk::DescriptorSetLayoutCreateFlags(), bindings));
        vk::PushConstantRange compute_push_constant(vk::ShaderStageFlagBits::eCompute,
                                                    0,
                                                    4 * sizeof(float));
        this->computePipelineLayout = ctx.device.createPipelineLayoutUnique(
            vk::PipelineLayoutCreateInfo(vk::PipelineLayoutCreateFlags(),
                                         *computeLayout,
                                         compute_push_constant));
        auto pipeline = ctx.device.createComputePipelineUnique(
            nullptr,
            vk::ComputePipelineCreateInfo(
                vk::PipelineCreateFlags(),
                vk::PipelineShaderStageCreateInfo(vk::PipelineShaderStageCreateFlags(),
                                                  vk::ShaderStageFlagBits::eCompute,
                                                  *module,
                                                  "main"),
                *this->computePipelineLayout));

        if (pipeline.result != vk::Result::eSuccess) {
            throw std::runtime_error("failed to create compute pipeline");
        }
        this->computePipeline = std::move(pipeline.value);

        vk::ShaderModule vertShader = loadShader(ctx, "instanced-rect.vert");
        vk::ShaderModule fragShader = loadShader(ctx, "instanced-rect.vert");
        std::vector shaderStages = {
            vk::PipelineShaderStageCreateInfo(vk::PipelineShaderStageCreateFlags(),
                                              vk::ShaderStageFlagBits::eVertex,
                                              vertShader,
                                              "main"),
            vk::PipelineShaderStageCreateInfo(vk::PipelineShaderStageCreateFlags(),
                                              vk::ShaderStageFlagBits::eFragment,
                                              fragShader,
                                              "main"),
        };

        vk::AttachmentDescription colorAttachment(vk::AttachmentDescriptionFlags(),
                                                  vk::Format::eR8G8B8A8Unorm,
                                                  vk::SampleCountFlagBits::e1,
                                                  vk::AttachmentLoadOp::eLoad,
                                                  vk::AttachmentStoreOp::eStore,
                                                  vk::AttachmentLoadOp::eDontCare,
                                                  vk::AttachmentStoreOp::eDontCare,
                                                  vk::ImageLayout::eColorAttachmentOptimal,
                                                  vk::ImageLayout::eColorAttachmentOptimal);
        vk::AttachmentReference colorAttachmentRef(0, vk::ImageLayout::eColorAttachmentOptimal);
        vk::SubpassDescription subpass(vk::SubpassDescriptionFlags(),
                                       vk::PipelineBindPoint::eGraphics,
                                       nullptr,
                                       colorAttachmentRef);
        vk::SubpassDependency dependency(vk::SubpassExternal,
                                         0,
                                         vk::PipelineStageFlagBits::eColorAttachmentOutput,
                                         vk::PipelineStageFlagBits::eColorAttachmentOutput,
                                         vk::AccessFlagBits::eNone,
                                         vk::AccessFlagBits::eColorAttachmentWrite);
        const vk::RenderPassCreateInfo renderPassInfo(vk::RenderPassCreateFlags(),
                                                      colorAttachment,
                                                      subpass,
                                                      dependency);
        this->renderPass = this->ctx.device.createRenderPassUnique(renderPassInfo);

        std::array<vk::DescriptorSetLayoutBinding, 2> drawBindings{};
        drawBindings[0].binding = 0;
        drawBindings[0].stageFlags = vk::ShaderStageFlagBits::eVertex;
        drawBindings[0].descriptorCount = 1;
        drawBindings[0].descriptorType = vk::DescriptorType::eStorageBuffer;
        drawBindings[1].binding = 0;
        drawBindings[1].stageFlags = vk::ShaderStageFlagBits::eFragment;
        drawBindings[1].descriptorCount = 1;
        drawBindings[1].descriptorType = vk::DescriptorType::eCombinedImageSampler;
        drawBindings[1].pImmutableSamplers = &this->sampler;
        vk::UniqueDescriptorSetLayout drawLayout = ctx.device.createDescriptorSetLayoutUnique(
            vk::DescriptorSetLayoutCreateInfo(vk::DescriptorSetLayoutCreateFlags(), drawBindings));
        vk::PushConstantRange draw_push_constant(vk::ShaderStageFlagBits::eVertex,
                                                 0,
                                                 3 * 3 * sizeof(float));
        this->drawPipelineLayout = ctx.device.createPipelineLayoutUnique(
            vk::PipelineLayoutCreateInfo(vk::PipelineLayoutCreateFlags(),
                                         *drawLayout,
                                         draw_push_constant));
        auto vertexInputInfo
            = vk::PipelineVertexInputStateCreateInfo(vk::PipelineVertexInputStateCreateFlags(),
                                                     nullptr,
                                                     nullptr);
        vk::PipelineInputAssemblyStateCreateInfo
            inputAssembly(vk::PipelineInputAssemblyStateCreateFlags(),
                          vk::PrimitiveTopology::eTriangleList,
                          false);
        std::vector dynamicStates = {vk::DynamicState::eViewport, vk::DynamicState::eScissor};

        vk::PipelineDynamicStateCreateInfo dynamicState(vk::PipelineDynamicStateCreateFlags(),
                                                        dynamicStates);

        vk::Viewport viewport{};
        vk::Rect2D scissor{};
        vk::PipelineViewportStateCreateInfo viewportState(vk::PipelineViewportStateCreateFlags(),
                                                          viewport,
                                                          scissor);
        vk::PipelineRasterizationStateCreateInfo
            rasterizer(vk::PipelineRasterizationStateCreateFlags(),
                       false,
                       false,
                       vk::PolygonMode::eFill,
                       vk::CullModeFlagBits::eBack,
                       vk::FrontFace::eClockwise,
                       false,
                       0.0f,
                       0.0f,
                       0.0f,
                       1.0f);
        vk::PipelineMultisampleStateCreateInfo
            multisampling(vk::PipelineMultisampleStateCreateFlags(),
                          vk::SampleCountFlagBits::e1,
                          false,
                          1.0f,
                          nullptr,
                          false,
                          false);

        vk::PipelineColorBlendAttachmentState
            colorBlendAttachment(false,
                                 vk::BlendFactor::eOne,
                                 vk::BlendFactor::eZero,
                                 vk::BlendOp::eAdd,
                                 vk::BlendFactor::eOne,
                                 vk::BlendFactor::eZero,
                                 vk::BlendOp::eAdd,
                                 vk::ColorComponentFlagBits::eR | vk::ColorComponentFlagBits::eG
                                     | vk::ColorComponentFlagBits::eB
                                     | vk::ColorComponentFlagBits::eA);
        vk::PipelineColorBlendStateCreateInfo colorBlending(vk::PipelineColorBlendStateCreateFlags(),
                                                            false,
                                                            vk::LogicOp::eCopy,
                                                            colorBlendAttachment,
                                                            {0.0, 0.0, 0.0, 0.0});
        vk::GraphicsPipelineCreateInfo drawPipelineInfo = {vk::PipelineCreateFlags(),
                                                           shaderStages,
                                                           &vertexInputInfo,
                                                           &inputAssembly,
                                                           {},
                                                           &viewportState,
                                                           &rasterizer,
                                                           &multisampling,
                                                           nullptr,
                                                           &colorBlending,
                                                           &dynamicState,
                                                           this->drawPipelineLayout.get(),
                                                           this->renderPass.get(),
                                                           0,
                                                           {},
                                                           -1};
        auto drawPipeline = ctx.device.createGraphicsPipelineUnique(nullptr, drawPipelineInfo);
        if (drawPipeline.result != vk::Result::eSuccess) {
            throw std::runtime_error("failed to create draw pipeline");
        }
        this->drawPipeline = std::move(drawPipeline.value);

        std::vector pool_sizes{vk::DescriptorPoolSize{vk::DescriptorType::eStorageBuffer, 3},
                               vk::DescriptorPoolSize{vk::DescriptorType::eUniformBuffer, 1}};
        this->descriptorPool = ctx.device.createDescriptorPoolUnique(
            vk::DescriptorPoolCreateInfo(vk::DescriptorPoolCreateFlags(), 5, pool_sizes));

        auto drawDescriptorSetAllocateLayoutBinding
            = vk::DescriptorSetLayoutBinding{0,
                                             vk::DescriptorType::eStorageBuffer,
                                             vk::ShaderStageFlagBits::eVertex,
                                             nullptr};
        vk::UniqueDescriptorSetLayout drawDescriptorSetAllocateLayout
            = ctx.device.createDescriptorSetLayoutUnique(
                vk::DescriptorSetLayoutCreateInfo(vk::DescriptorSetLayoutCreateFlags(),
                                                  drawDescriptorSetAllocateLayoutBinding));
        std::vector descriptorSetLayouts{*computeLayout, *drawDescriptorSetAllocateLayout};
        this->descriptorSets = ctx.device.allocateDescriptorSetsUnique(
            vk::DescriptorSetAllocateInfo(*this->descriptorPool, descriptorSetLayouts));
        auto descriptorSetsView = this->descriptorSets
                                  | std::ranges::views::transform(
                                      [](vk::UniqueDescriptorSet& set) { return *set; });

        std::vector<vk::DescriptorBufferInfo>
            descriptorBufferInfos{{*this->instancesBuffer, 0, vk::WholeSize},
                                  {*this->visibleInstancesBuffer, 0, vk::WholeSize},
                                  {*this->drawCountBuffer, 0, vk::WholeSize},
                                  {*this->visibleInstancesBuffer, 0, vk::WholeSize}};
        std::vector writeDescriptorSets{vk::WriteDescriptorSet{*this->descriptorSets[0],
                                                               0,
                                                               0,
                                                               vk::DescriptorType::eStorageBuffer,
                                                               nullptr,
                                                               descriptorBufferInfos[0],
                                                               nullptr},
                                        vk::WriteDescriptorSet{*this->descriptorSets[1],
                                                               1,
                                                               0,
                                                               vk::DescriptorType::eStorageBuffer,
                                                               nullptr,
                                                               descriptorBufferInfos[1],
                                                               nullptr},
                                        vk::WriteDescriptorSet{*this->descriptorSets[2],
                                                               2,
                                                               0,
                                                               vk::DescriptorType::eStorageBuffer,
                                                               nullptr,
                                                               descriptorBufferInfos[2],
                                                               nullptr},
                                        vk::WriteDescriptorSet{*this->descriptorSets[3],
                                                               0,
                                                               0,
                                                               vk::DescriptorType::eStorageBuffer,
                                                               nullptr,
                                                               descriptorBufferInfos[3],
                                                               nullptr}};
        this->ctx.device.updateDescriptorSets(writeDescriptorSets, nullptr);

        this->drawCommandBuffers = std::move(ctx.device.allocateCommandBuffersUnique(
            vk::CommandBufferAllocateInfo(*this->drawCommandPool,
                                          vk::CommandBufferLevel::ePrimary,
                                          board.image_count)));
        this->computeCommandBuffer = std::move(ctx.device.allocateCommandBuffersUnique(
            vk::CommandBufferAllocateInfo(*this->computeCommandPool,
                                          vk::CommandBufferLevel::ePrimary,
                                          1))[0]);

        this->computeCommandBuffer->begin(vk::CommandBufferBeginInfo());
        this->computeCommandBuffer->bindPipeline(vk::PipelineBindPoint::eCompute,
                                                 *this->computePipeline);
        std::vector<vk::DescriptorSet> descriptorSets{descriptorSetsView.begin(),
                                                      descriptorSetsView.begin() + 3};
        this->computeCommandBuffer->bindDescriptorSets(vk::PipelineBindPoint::eCompute,
                                                       *this->computePipelineLayout,
                                                       0,
                                                       descriptorSets,
                                                       {});
        std::vector<instance> constants{{0, 0, 500, 500}};
        this->computeCommandBuffer->pushConstants(*this->computePipelineLayout,
                                                  vk::ShaderStageFlagBits::eCompute,
                                                  0,
                                                  constants.size(),
                                                  constants.data());
        this->computeCommandBuffer->dispatch(glm::ceil(BUFFER_SIZE / WORKGROUP_SIZE), 1, 1);
        this->computeCommandBuffer->end();

        for (int i = 0; i < this->board.image_count; ++i) {
            this->recordCommandBuffer(i);
        }
    }

    void recordCommandBuffer(const uint8_t frame) {
        this->drawCommandBuffers[frame]->begin(vk::CommandBufferBeginInfo());
        std::vector clearValues{vk::ClearValue{vk::ClearColorValue{0.0f, 0.0f, 0.0f, 1.0f}}};
        const vk::RenderPassBeginInfo beginRenderPassInfo(*this->renderPass,
                                                          *board.images[frame].framebuffer,
                                                          vk::Rect2D({0, 0},
                                                                     vk::Extent2D{board.width,
                                                                                  board.height}),
                                                          clearValues);
        this->drawCommandBuffers[frame]->beginRenderPass(beginRenderPassInfo,
                                                         vk::SubpassContents::eInline);
        this->drawCommandBuffers[frame]->bindPipeline(vk::PipelineBindPoint::eGraphics,
                                                      *this->drawPipeline);

        this->computeCommandBuffer->bindDescriptorSets(vk::PipelineBindPoint::eCompute,
                                                       *this->computePipelineLayout,
                                                       0,
                                                       *this->descriptorSets[3],
                                                       {});
        const vk::Viewport viewport(0.0f,
                                    0.0f,
                                    static_cast<float>(this->board.width),
                                    static_cast<float>(this->board.height),
                                    0.0f,
                                    1.0f);
        this->drawCommandBuffers[frame]->setViewport(0, viewport);
        const vk::Rect2D scissor({0, 0}, vk::Extent2D{this->board.width, this->board.height});
        this->drawCommandBuffers[frame]->setScissor(0, scissor);

        this->drawCommandBuffers[frame]->draw(6, BUFFER_SIZE, 0, 0);

        this->drawCommandBuffers[frame]->endRenderPass();
        this->drawCommandBuffers[frame]->end();
    }

    void on_resize(const uint8_t frame) {
        this->drawCommandBuffers[frame]->reset();
        this->recordCommandBuffer(frame);
    }

private:
    const vulkan_context& ctx;
    const circuit_board& board;
    const vk::Sampler& sampler;

    vk::UniqueCommandPool computeCommandPool;
    vk::UniqueCommandPool drawCommandPool;

    vk::UniqueBuffer instancesBuffer;
    vk::UniqueBuffer visibleInstancesBuffer;
    vk::UniqueBuffer drawCountBuffer;
    vk::UniquePipelineLayout computePipelineLayout;
    vk::UniquePipeline computePipeline;

    vk::UniqueRenderPass renderPass;
    vk::UniquePipelineLayout drawPipelineLayout;
    vk::UniquePipeline drawPipeline;

    vk::UniqueDescriptorPool descriptorPool;
    std::vector<vk::UniqueDescriptorSet> descriptorSets;
};

#endif //INDIRECT_RENDERER_H
