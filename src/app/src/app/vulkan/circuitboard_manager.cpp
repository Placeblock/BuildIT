//
// Created by felix on 18.07.25.
//

#include "app/vulkan/circuitboard_manager.hpp"

#include <filesystem>
#include <fstream>

const std::filesystem::path SHADER_DIR = {"shaders/"};

const uint32_t MAX_CIRCUIT_BOARDS = 32;
const uint32_t IMAGES_PER_BOARD = 3;

static std::string readShader(const std::string &filename) {
    std::filesystem::path path = SHADER_DIR / filename;
    std::ifstream file(path, std::ifstream::in);
    if (!file.is_open()) {
        throw std::runtime_error("failed to open file!");
    }
    std::stringstream buffer;
    buffer << file.rdbuf();

    return buffer.str();
}

circuitboard_manager::circuitboard_manager(const vulkan_context &ctx) : ctx(ctx) {
    this->create_descriptor_pool();
    this->create_sampler();
    this->create_command_pool();
    this->create_descriptor_set_layout();
    this->create_render_pass();
    this->create_pipeline();

    for (int i = 0; i < IMAGES_PER_BOARD; ++i) {
        this->in_flight_fences.push_back(std::move(this->ctx.device.createFenceUnique(
            vk::FenceCreateInfo{vk::FenceCreateFlagBits::eSignaled})));
    }
    this->render_finished_semaphore = this->ctx.device.createSemaphoreUnique(
        vk::SemaphoreCreateInfo{});
}

circuit_board *circuitboard_manager::create_board() {
    auto board = std::make_unique<circuit_board>(this->ctx,
                                                 this->render_pass.get(),
                                                 this->pipeline.get(),
                                                 this->descriptor_pool.get(),
                                                 this->descriptor_set_layout.get(),
                                                 this->sampler.get(),
                                                 this->command_pool.get(),
                                                 400,
                                                 400,
                                                 IMAGES_PER_BOARD);
    this->circuit_boards.push_back(std::move(board));
    return this->circuit_boards.back().get();
}

void circuitboard_manager::render(const vk::Queue &queue, const uint32_t in_flight_frame) {
    if (this->ctx.device.waitForFences(this->in_flight_fences[in_flight_frame].get(),
                                       vk::True,
                                       UINT64_MAX)
        != vk::Result::eSuccess) {
        throw std::runtime_error("failed to wait for the circuit board fence");
    }
    this->ctx.device.resetFences(this->in_flight_fences[in_flight_frame].get());
    std::vector<vk::CommandBuffer> command_buffers;
    for (const auto &board : this->circuit_boards) {
        command_buffers.push_back(board->command_buffers[in_flight_frame].get());
    }
    const vk::SubmitInfo submitInfo{nullptr,
                                    nullptr,
                                    command_buffers,
                                    this->render_finished_semaphore.get()};
    queue.submit(submitInfo, this->in_flight_fences[in_flight_frame].get());
}

bool circuitboard_manager::can_resize() {
    for (int i = 0; i < IMAGES_PER_BOARD; ++i) {
        if (this->ctx.device.getFenceStatus(this->in_flight_fences[i].get()) != vk::Result::eSuccess)
            return false;
    }
    return true;
}

void circuitboard_manager::create_descriptor_pool() {
    constexpr uint32_t MAX_SETS = MAX_CIRCUIT_BOARDS * IMAGES_PER_BOARD;
    std::vector poolSizes = {
        vk::DescriptorPoolSize{vk::DescriptorType::eCombinedImageSampler, MAX_SETS}};
    this->descriptor_pool = std::move(ctx.device.createDescriptorPoolUnique(
        vk::DescriptorPoolCreateInfo{vk::DescriptorPoolCreateFlags(), MAX_SETS, poolSizes}));
}

void circuitboard_manager::create_sampler() {
    this->sampler = ctx.device.createSamplerUnique(vk::SamplerCreateInfo{});
}

void circuitboard_manager::create_command_pool() {
    this->command_pool = ctx.device.createCommandPoolUnique(
        vk::CommandPoolCreateInfo{vk::CommandPoolCreateFlagBits::eResetCommandBuffer,
                                  this->ctx.queue_families.graphics_family});
}

void circuitboard_manager::create_descriptor_set_layout() {
    std::vector bindings = {vk::DescriptorSetLayoutBinding{0,
                                                           vk::DescriptorType::eCombinedImageSampler,
                                                           1,
                                                           vk::ShaderStageFlagBits::eFragment,
                                                           &this->sampler.get()}};
    this->descriptor_set_layout = this->ctx.device.createDescriptorSetLayoutUnique(
        vk::DescriptorSetLayoutCreateInfo{vk::DescriptorSetLayoutCreateFlags(), bindings});
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

vk::ShaderModule circuitboard_manager::createShaderModule(const std::string &code) const {
    const vk::ShaderModuleCreateInfo createInfo = {vk::ShaderModuleCreateFlags{},
                                                   code.size(),
                                                   reinterpret_cast<const uint32_t *>(code.data()),
                                                   nullptr};
    return this->ctx.device.createShaderModule(createInfo);
}

void circuitboard_manager::create_pipeline() {
    const std::string vertShader = readShader("grid.vert.spv");
    const std::string fragShader = readShader("grid.frag.spv");

    const vk::ShaderModule vertModule = this->createShaderModule(vertShader);
    const vk::ShaderModule fragModule = this->createShaderModule(fragShader);

    vk::PipelineShaderStageCreateInfo vertShaderStageInfo(vk::PipelineShaderStageCreateFlags(),
                                                          vk::ShaderStageFlagBits::eVertex,
                                                          vertModule,
                                                          "main");
    vk::PipelineShaderStageCreateInfo fragShaderStageInfo(vk::PipelineShaderStageCreateFlags(),
                                                          vk::ShaderStageFlagBits::eFragment,
                                                          fragModule,
                                                          "main");
    vk::PipelineShaderStageCreateInfo shaderStages[] = {vertShaderStageInfo, fragShaderStageInfo};
    vk::PipelineVertexInputStateCreateInfo vertexInputInfo(vk::PipelineVertexInputStateCreateFlags(),
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