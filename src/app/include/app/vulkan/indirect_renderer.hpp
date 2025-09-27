//
// Created by felix on 8/3/25.
//

#ifndef INDIRECT_RENDERER_H
#define INDIRECT_RENDERER_H

#include "circuitboard.hpp"
#include "memory.hpp"
#include "shader.hpp"
#include "vulkancontext.hpp"
#include <glm/glm.hpp>
#include <iostream>
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
    std::vector<vk::UniqueCommandBuffer> computeCommandBuffers;
    std::vector<vk::UniqueCommandBuffer> drawCommandBuffers;

    explicit indirect_renderer(const circuit_board& board,
                               const vk::Sampler& sampler,
                               const vulkan_context& ctx)
        : ctx(ctx), board(board), sampler(sampler) {
        /**
         * We instantiate the Command Pool. This Command Pool is going to later be used to instantiate the
         * CommandBuffers used for the compute stage
        */
        this->drawCommandPool = ctx.device.createCommandPoolUnique(
            vk::CommandPoolCreateInfo{vk::CommandPoolCreateFlagBits::eResetCommandBuffer,
                                      this->ctx.queue_families.graphics_family});
        this->computeCommandPool = ctx.device.createCommandPoolUnique(
            vk::CommandPoolCreateInfo{vk::CommandPoolCreateFlagBits::eResetCommandBuffer,
                                      this->ctx.queue_families.compute_family});

        /**
         * We allocate the InstancesBuffer. This buffer is going to hold all instances which could be
         * visible.
         * We use UsageFlagBits of StorageBuffer, because we want to access it in the Compute Shader
         * as a SSBO. SSBO is necessary because of the amount of data.
         * We also declare which queue families are going to use the buffer. This is necessary for
         * Memory optimization
         */
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

        /**
         * We also create a very small buffer which only holds one integer. This buffer will
         * be used to track the amount of Items in the visible instances buffer.
         * It can later be used together with a indirect count extension to directly use the
         * data from the visible instances buffer for rendering.
         */
        std::vector draw_count_queue_families = {ctx.queue_families.compute_family};
        this->drawCountBuffer = ctx.device.createBufferUnique(
            vk::BufferCreateInfo(vk::BufferCreateFlags(),
                                 sizeof(uint32_t),
                                 vk::BufferUsageFlagBits::eStorageBuffer,
                                 vk::SharingMode::eExclusive,
                                 draw_count_queue_families));

        /**
         * WE should check the memory requirements of all three shaders.
         * However, we allocate memory for all three buffers.
         */
        const vk::MemoryRequirements instances_memory_requirements
            = ctx.device.getBufferMemoryRequirements(this->instancesBuffer.get());
        const vk::MemoryRequirements visible_instances_memory_requirements
            = ctx.device.getBufferMemoryRequirements(this->visibleInstancesBuffer.get());
        const vk::MemoryRequirements draw_count_memory_requirements
            = ctx.device.getBufferMemoryRequirements(this->drawCountBuffer.get());
        const uint32_t image_memory_type
            = find_memory_type(ctx,
                               instances_memory_requirements.memoryTypeBits,
                               vk::MemoryPropertyFlagBits::eDeviceLocal);
        const vk::MemoryAllocateInfo
            memory_allocate_info{instances_memory_requirements.size
                                     + visible_instances_memory_requirements.size
                                     + draw_count_memory_requirements.size,
                                 image_memory_type};
        this->bufferMemory = ctx.device.allocateMemoryUnique(memory_allocate_info);
        ctx.device.bindBufferMemory(this->instancesBuffer.get(), this->bufferMemory.get(), 0);
        ctx.device.bindBufferMemory(this->visibleInstancesBuffer.get(),
                                    this->bufferMemory.get(),
                                    instances_memory_requirements.size);
        ctx.device.bindBufferMemory(this->drawCountBuffer.get(),
                                    this->bufferMemory.get(),
                                    instances_memory_requirements.size
                                        + visible_instances_memory_requirements.size);

        auto module = loadShaderUnique(ctx, "rect-culling.comp.spv");

        /**
         * After loading the shader, we have to properly set the bindings.
         * The descriptorCount is always 1 as we do not use descriptor arrays inside the shader.
         * This descriptorsetlayoutbindings describe the bindings (Individual descriptors) for
         * the compute pipeline
         */
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

        /**
         * Create the Compute Layout using the compute bindings.
         */
        vk::UniqueDescriptorSetLayout computeLayout = ctx.device.createDescriptorSetLayoutUnique(
            vk::DescriptorSetLayoutCreateInfo(vk::DescriptorSetLayoutCreateFlags(), bindings));
        /**
         * For the compute shader we also use a push constant to send the current viewport to be able
         * to do culling
         */
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

        /**
         * Load the vertex and fragment shaders for rendering the culled rectangles.
         */
        vk::UniqueShaderModule vertShader = loadShaderUnique(ctx, "instanced-rect.vert.spv");
        vk::UniqueShaderModule fragShader = loadShaderUnique(ctx, "instanced-rect.frag.spv");
        std::vector shaderStages = {
            vk::PipelineShaderStageCreateInfo(vk::PipelineShaderStageCreateFlags(),
                                              vk::ShaderStageFlagBits::eVertex,
                                              *vertShader,
                                              "main"),
            vk::PipelineShaderStageCreateInfo(vk::PipelineShaderStageCreateFlags(),
                                              vk::ShaderStageFlagBits::eFragment,
                                              *fragShader,
                                              "main"),
        };

        /**
         * We create a color attachment for the rendered image. It uses loadop dontcare, because we
         * do not care about old data. We use storeop eStore to persist the new data.
         * For this we create a subpass which is used for graphics rendering. It uses the colorAttachment.
         */
        vk::AttachmentDescription colorAttachment(vk::AttachmentDescriptionFlags(),
                                                  vk::Format::eR8G8B8A8Unorm,
                                                  vk::SampleCountFlagBits::e1,
                                                  vk::AttachmentLoadOp::eDontCare,
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
        /**
         * This dependency ensures, that all writes to color attachments of previous subpasses
         * are already done. This prevents subpass 0 from starting too early and potentially
         * overwriting or conflicting with previous work that touched the same images.
         */
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

        /**
         * We create another descriptorsetlayout for the graphics pipeline.
         * This descriptorset will have index 2, the first one index 1.
         * It only includes the buffer containing visible instances for rendering.
         */
        std::array<vk::DescriptorSetLayoutBinding, 1> drawBindings{};
        drawBindings[0].binding = 0;
        drawBindings[0].stageFlags = vk::ShaderStageFlagBits::eVertex;
        drawBindings[0].descriptorCount = 1;
        drawBindings[0].descriptorType = vk::DescriptorType::eStorageBuffer;
        vk::UniqueDescriptorSetLayout drawLayout = ctx.device.createDescriptorSetLayoutUnique(
            vk::DescriptorSetLayoutCreateInfo(vk::DescriptorSetLayoutCreateFlags(), drawBindings));
        /**
         * We also create the push constant for the mat3 matrix which is the projection matrix.
         */
        vk::PushConstantRange draw_push_constant(vk::ShaderStageFlagBits::eVertex,
                                                 0,
                                                 3 * 3 * sizeof(float));
        this->drawPipelineLayout = ctx.device.createPipelineLayoutUnique(
            vk::PipelineLayoutCreateInfo(vk::PipelineLayoutCreateFlags(),
                                         *drawLayout,
                                         draw_push_constant));
        /**
         * We now need to create some structs describing the vertex data the vertex shader receives.
         * The vertex buffer is just some bytes and vulkan needs to know how to interpret that data
         */
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
        /**
         * We now have the pipeline!
         */
        this->drawPipeline = std::move(drawPipeline.value);

        /**
         * Now that we have created the layouts for everything and pipelines, we can start instantiating
         * the individual buffers.
         */
        std::vector pool_sizes{vk::DescriptorPoolSize{vk::DescriptorType::eStorageBuffer, 4}};
        this->descriptorPool = ctx.device.createDescriptorPoolUnique(
            vk::DescriptorPoolCreateInfo(vk::DescriptorPoolCreateFlags(), 4, pool_sizes));

        /**
         * We allocate the descriptorsets using the descriptorsetlayouts and the descriptorpool
         */
        std::vector descriptorSetLayouts{*computeLayout, *drawLayout};
        std::vector<vk::UniqueDescriptorSet> descriptor_sets
            = ctx.device.allocateDescriptorSetsUnique(
                vk::DescriptorSetAllocateInfo(*this->descriptorPool, descriptorSetLayouts));
        this->computeDescriptorSet = std::move(descriptor_sets[0]);
        this->graphicsDescriptorSet = std::move(descriptor_sets[1]);

        /**
         * We then tell vulkan which buffers to use for each descriptor and update the sets
         */
        vk::DescriptorBufferInfo instancesDescriptorBufferInfo{*this->instancesBuffer,
                                                               0,
                                                               vk::WholeSize};
        vk::DescriptorBufferInfo visibleInstancesDescriptorBufferInfo{*this->instancesBuffer,
                                                                      0,
                                                                      vk::WholeSize};
        vk::DescriptorBufferInfo drawCountDescriptorBufferInfo{*this->instancesBuffer,
                                                               0,
                                                               vk::WholeSize};
        std::vector writeDescriptorSets{vk::WriteDescriptorSet{*this->computeDescriptorSet,
                                                               0,
                                                               0,
                                                               vk::DescriptorType::eStorageBuffer,
                                                               nullptr,
                                                               instancesDescriptorBufferInfo,
                                                               nullptr},
                                        vk::WriteDescriptorSet{*this->computeDescriptorSet,
                                                               1,
                                                               0,
                                                               vk::DescriptorType::eStorageBuffer,
                                                               nullptr,
                                                               visibleInstancesDescriptorBufferInfo,
                                                               nullptr},
                                        vk::WriteDescriptorSet{*this->computeDescriptorSet,
                                                               2,
                                                               0,
                                                               vk::DescriptorType::eStorageBuffer,
                                                               nullptr,
                                                               drawCountDescriptorBufferInfo,
                                                               nullptr},
                                        vk::WriteDescriptorSet{*this->graphicsDescriptorSet,
                                                               0,
                                                               0,
                                                               vk::DescriptorType::eStorageBuffer,
                                                               nullptr,
                                                               visibleInstancesDescriptorBufferInfo,
                                                               nullptr}};
        this->ctx.device.updateDescriptorSets(writeDescriptorSets, nullptr);

        /**
         * We allocate draw and compute command buffers for each image
         */
        this->drawCommandBuffers = std::move(ctx.device.allocateCommandBuffersUnique(
            vk::CommandBufferAllocateInfo(*this->drawCommandPool,
                                          vk::CommandBufferLevel::ePrimary,
                                          board.image_count)));
        this->computeCommandBuffers = std::move(ctx.device.allocateCommandBuffersUnique(
            vk::CommandBufferAllocateInfo(*this->computeCommandPool,
                                          vk::CommandBufferLevel::ePrimary,
                                          board.image_count)));

        /**
         * Record the individual command buffers
         */
        for (int i = 0; i < this->board.image_count; ++i) {
            this->recordCommandBuffer(i);
        }
    }

    /*indirect_renderer(indirect_renderer& other)
        : ctx(other.ctx), board(other.board), sampler(other.sampler) {
        this->computeCommandPool = std::move(other.computeCommandPool);
        this->drawCommandPool = std::move(other.drawCommandPool);
        this->instancesBuffer = std::move(other.instancesBuffer);
        this->visibleInstancesBuffer = std::move(other.visibleInstancesBuffer);
        this->drawCountBuffer = std::move(other.drawCountBuffer);
        this->computePipelineLayout = std::move(other.computePipelineLayout);
        this->computePipeline = std::move(other.computePipeline);

        this->renderPass = std::move(other.renderPass);
        this->drawPipelineLayout = std::move(other.drawPipelineLayout);
        this->drawPipeline = std::move(other.drawPipeline);

        this->descriptorPool = std::move(other.descriptorPool);
        this->descriptorSets = other.descriptorSets;
    }*/

    void recordCommandBuffer(const uint8_t frame) {
        this->computeCommandBuffers[frame]->begin(vk::CommandBufferBeginInfo());
        this->computeCommandBuffers[frame]->bindPipeline(vk::PipelineBindPoint::eCompute,
                                                         *this->computePipeline);

        this->computeCommandBuffers[frame]->bindDescriptorSets(vk::PipelineBindPoint::eCompute,
                                                               *this->computePipelineLayout,
                                                               0,
                                                               *this->computeDescriptorSet,
                                                               {});
        const std::vector<instance> constants{{0, 0, 500, 500}};
        this->computeCommandBuffers[frame]->pushConstants(*this->computePipelineLayout,
                                                          vk::ShaderStageFlagBits::eCompute,
                                                          0,
                                                          constants.size() * sizeof(instance),
                                                          constants.data());
        this->computeCommandBuffers[frame]->dispatch(glm::ceil(BUFFER_SIZE / WORKGROUP_SIZE), 1, 1);
        this->computeCommandBuffers[frame]->end();

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

        this->drawCommandBuffers[frame]->bindDescriptorSets(vk::PipelineBindPoint::eGraphics,
                                                            *this->drawPipelineLayout,
                                                            0,
                                                            *this->graphicsDescriptorSet,
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

        const std::vector draw_constants{glm::mat3(1.0f)};
        this->drawCommandBuffers[frame]->pushConstants(*this->drawPipelineLayout,
                                                       vk::ShaderStageFlagBits::eVertex,
                                                       0,
                                                       draw_constants.size() * sizeof(glm::mat3),
                                                       draw_constants.data());
        this->drawCommandBuffers[frame]->draw(6, BUFFER_SIZE, 0, 0);

        this->drawCommandBuffers[frame]->endRenderPass();
        this->drawCommandBuffers[frame]->end();
    }

    void on_resize(const uint8_t frame) {
        this->drawCommandBuffers[frame]->reset();
        this->recordCommandBuffer(frame);
    }

    ~indirect_renderer() {
        std::cout << "DESTROYING INDIRECT RENDERER" << std::endl;
        this->ctx.device.freeDescriptorSets(*this->descriptorPool, *this->computeDescriptorSet);
        this->ctx.device.freeDescriptorSets(*this->descriptorPool, *this->graphicsDescriptorSet);
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
    vk::UniqueDeviceMemory bufferMemory;

    vk::UniquePipelineLayout computePipelineLayout;
    vk::UniquePipeline computePipeline;

    vk::UniqueRenderPass renderPass;
    vk::UniquePipelineLayout drawPipelineLayout;
    vk::UniquePipeline drawPipeline;

    vk::UniqueDescriptorPool descriptorPool;
    vk::UniqueDescriptorSet computeDescriptorSet;
    vk::UniqueDescriptorSet graphicsDescriptorSet;
};

#endif //INDIRECT_RENDERER_H
