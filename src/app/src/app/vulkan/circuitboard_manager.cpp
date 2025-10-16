//
// Created by felix on 18.07.25.
//

#include "app/vulkan/circuitboard_manager.hpp"

#include "app/vulkan/shader.hpp"
#include "spdlog/spdlog.h"

const uint32_t MAX_CIRCUIT_BOARDS = 32;

circuitboard_manager::circuitboard_manager(const vulkan_context &ctx,
                                           const uint32_t in_flight_frames)
    : in_flight_frames(in_flight_frames), ctx(ctx) {
    this->create_sampler();
    this->create_command_pool();
    this->create_render_pass();
    this->create_pipeline();

    vk::CommandBufferAllocateInfo compute_buffer_info{
        *this->compute_command_pool, vk::CommandBufferLevel::ePrimary, in_flight_frames};
    vk::CommandBufferAllocateInfo graphics_buffer_info{
        *this->graphics_command_pool, vk::CommandBufferLevel::ePrimary, in_flight_frames};
    auto compute_buffers = ctx.device.allocateCommandBuffersUnique(compute_buffer_info);
    auto graphics_buffers = ctx.device.allocateCommandBuffersUnique(graphics_buffer_info);
    for (int i = 0; i < in_flight_frames; ++i) {
        auto fence = this->ctx.device.createFenceUnique(
            vk::FenceCreateInfo{vk::FenceCreateFlagBits::eSignaled});

        vk::UniqueSemaphore compute_finished_semaphore = ctx.device.createSemaphoreUnique({
            vk::SemaphoreCreateFlagBits{}
        });
        vk::UniqueSemaphore render_finished_semaphore = ctx.device.createSemaphoreUnique({
            vk::SemaphoreCreateFlagBits{}
        });

        frame_resource resource = {
            std::move(fence),
            std::move(compute_finished_semaphore),
            std::move(render_finished_semaphore),
            std::move(graphics_buffers[i]),
            std::move(compute_buffers[i]),
        };

        this->frame_resources.push_back(std::move(resource));
    }
}

circuit_board *circuitboard_manager::create_board() {
    auto board = std::make_unique<circuit_board>(this->ctx,
                                                 *this->render_pass,
                                                 *this->pipeline,
                                                 *this->sampler,
                                                 400,
                                                 400,
                                                 this->in_flight_frames);
    this->circuit_boards.push_back(std::move(board));
    return this->circuit_boards.back().get();
}

void circuitboard_manager::render(const vk::Queue &compute_queue,
                                  const vk::Queue &graphics_queue,
                                  const uint32_t in_flight_frame) {
    if (this->ctx.device.waitForFences(*this->frame_resources[in_flight_frame].preflight_fence,
                                       vk::True,
                                       UINT64_MAX)
        != vk::Result::eSuccess) {
        throw std::runtime_error("failed to wait for the circuit board fence");
    }
    this->ctx.device.resetFences(*this->frame_resources[in_flight_frame].preflight_fence);

    auto compute_command_buffer = *this->frame_resources[in_flight_frame].compute_buffer;
    auto graphics_command_buffer = *this->frame_resources[in_flight_frame].graphics_buffer;
    compute_command_buffer.begin({vk::CommandBufferUsageFlagBits::eOneTimeSubmit});
    graphics_command_buffer.begin({vk::CommandBufferUsageFlagBits::eOneTimeSubmit});
    for (const auto &board : this->circuit_boards) {
        board->record(compute_command_buffer,
                      graphics_command_buffer,
                      in_flight_frame);
    }
    compute_command_buffer.end();
    graphics_command_buffer.end();

    spdlog::debug("Submitting compute commands for circuitboards");
    const vk::SubmitInfo compute_submit_info{nullptr,
                                             nullptr,
                                             compute_command_buffer,
                                             *this->frame_resources[in_flight_frame].
                                             compute_finished_semaphore};
    compute_queue.submit(compute_submit_info, nullptr);

    spdlog::debug("Submitting graphics commands for circuitboards");
    constexpr vk::PipelineStageFlags wait_stage = vk::PipelineStageFlagBits::eVertexShader;
    const vk::SubmitInfo graphics_submit_info{*this->frame_resources[in_flight_frame].
                                              compute_finished_semaphore,
                                              wait_stage,
                                              graphics_command_buffer,
                                              *this->frame_resources[in_flight_frame].
                                              render_finished_semaphore};
    graphics_queue.submit(graphics_submit_info,
                          *this->frame_resources[in_flight_frame].preflight_fence);
}

bool circuitboard_manager::can_resize(const uint32_t image_index) {
    return this->ctx.device.getFenceStatus(*this->frame_resources[image_index].preflight_fence)
           == vk::Result::eSuccess;
}

void circuitboard_manager::create_sampler() {
    this->sampler = ctx.device.createSamplerUnique(vk::SamplerCreateInfo{});
}

void circuitboard_manager::create_command_pool() {
    this->graphics_command_pool = ctx.device.createCommandPoolUnique(
        vk::CommandPoolCreateInfo{vk::CommandPoolCreateFlagBits::eResetCommandBuffer,
                                  this->ctx.queue_families.graphics_family});
    this->compute_command_pool = ctx.device.createCommandPoolUnique(
        vk::CommandPoolCreateInfo{vk::CommandPoolCreateFlagBits::eResetCommandBuffer,
                                  this->ctx.queue_families.compute_family});
}

void circuitboard_manager::create_render_pass() {
    vk::AttachmentDescription colorAttachment(vk::AttachmentDescriptionFlags(),
                                              vk::Format::eR8G8B8A8Unorm,
                                              vk::SampleCountFlagBits::e1,
                                              vk::AttachmentLoadOp::eDontCare,
                                              vk::AttachmentStoreOp::eStore,
                                              vk::AttachmentLoadOp::eDontCare,
                                              vk::AttachmentStoreOp::eDontCare,
                                              vk::ImageLayout::eUndefined,
                                              vk::ImageLayout::eShaderReadOnlyOptimal);
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
    this->render_pass = this->ctx.device.createRenderPassUnique(renderPassInfo);
}

void circuitboard_manager::create_pipeline() {
    const std::vector<uint32_t> vertShader = readShader("grid.vert.spv");
    const std::vector<uint32_t> fragShader = readShader("grid.frag.spv");

    const vk::ShaderModule vertModule = createShaderModule(this->ctx, vertShader);
    const vk::ShaderModule fragModule = createShaderModule(this->ctx, fragShader);

    vk::PipelineShaderStageCreateInfo vertShaderStageInfo(vk::PipelineShaderStageCreateFlags(),
                                                          vk::ShaderStageFlagBits::eVertex,
                                                          vertModule,
                                                          "main");
    vk::PipelineShaderStageCreateInfo fragShaderStageInfo(vk::PipelineShaderStageCreateFlags(),
                                                          vk::ShaderStageFlagBits::eFragment,
                                                          fragModule,
                                                          "main");
    vk::PipelineShaderStageCreateInfo shaderStages[] = {vertShaderStageInfo, fragShaderStageInfo};
    vk::PipelineVertexInputStateCreateInfo vertexInputInfo(
        vk::PipelineVertexInputStateCreateFlags(),
        nullptr,
        nullptr);
    vk::PipelineInputAssemblyStateCreateInfo
        inputAssembly(vk::PipelineInputAssemblyStateCreateFlags(),
                      vk::PrimitiveTopology::eTriangleList,
                      false);
    vk::Viewport viewport{};
    vk::Rect2D scissor{};

    std::vector dynamicStates = {vk::DynamicState::eViewport, vk::DynamicState::eScissor};

    vk::PipelineDynamicStateCreateInfo dynamicState(vk::PipelineDynamicStateCreateFlags(),
                                                    dynamicStates);

    vk::PipelineViewportStateCreateInfo viewportState(vk::PipelineViewportStateCreateFlags(),
                                                      viewport,
                                                      scissor);

    vk::PipelineRasterizationStateCreateInfo rasterizer(vk::PipelineRasterizationStateCreateFlags(),
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
    vk::PipelineMultisampleStateCreateInfo multisampling(vk::PipelineMultisampleStateCreateFlags(),
                                                         vk::SampleCountFlagBits::e1,
                                                         false,
                                                         1.0f,
                                                         nullptr,
                                                         false,
                                                         false);

    vk::PipelineColorBlendAttachmentState colorBlendAttachment(false,
                                                               vk::BlendFactor::eOne,
                                                               vk::BlendFactor::eZero,
                                                               vk::BlendOp::eAdd,
                                                               vk::BlendFactor::eOne,
                                                               vk::BlendFactor::eZero,
                                                               vk::BlendOp::eAdd,
                                                               vk::ColorComponentFlagBits::eR
                                                               | vk::ColorComponentFlagBits::eG
                                                               | vk::ColorComponentFlagBits::eB
                                                               | vk::ColorComponentFlagBits::eA);
    vk::PipelineColorBlendStateCreateInfo colorBlending(vk::PipelineColorBlendStateCreateFlags(),
                                                        false,
                                                        vk::LogicOp::eCopy,
                                                        colorBlendAttachment,
                                                        {0.0, 0.0, 0.0, 0.0});
    vk::PipelineLayoutCreateInfo layoutInfo(vk::PipelineLayoutCreateFlags(), nullptr, nullptr);
    this->pipeline_layout = this->ctx.device.createPipelineLayoutUnique(layoutInfo);

    vk::GraphicsPipelineCreateInfo pipelineInfo(vk::PipelineCreateFlags(),
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
                                                this->pipeline_layout.get(),
                                                this->render_pass.get(),
                                                0,
                                                {},
                                                -1);
    auto graphics_pipeline = this->ctx.device.createGraphicsPipelineUnique(nullptr, pipelineInfo);
    if (graphics_pipeline.result != vk::Result::eSuccess) {
        throw std::runtime_error("failed to create graphics pipeline");
    }
    this->pipeline = std::move(graphics_pipeline.value);

    this->ctx.device.destroyShaderModule(vertModule);
    this->ctx.device.destroyShaderModule(fragModule);
}