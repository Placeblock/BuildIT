//
// Created by felix on 12/25/25.
//
#define GLFW_INCLUDE_VULKAN

#define VMA_VULKAN_VERSION 1002000 // Vulkan 1.2
#define VMA_IMPLEMENTATION

#include "chip_buffer.hpp"
#include "bit/queue_family.hpp"
#include "GLFW/glfw3.h"
#include "bit/debug-names.hpp"
#include <vulkan/vulkan.hpp>
#include "spdlog/spdlog.h"
#include <map>
#include <set>
#include "bit/pipeline/build_pipeline.hpp"
#include "bit/shader.hpp"
#include "bit/memory/memory_allocator.hpp"
#include "bit/pipeline/compute_pipeline_builder.hpp"
#include "bit/pipeline/descriptor_set_layout_builder.hpp"
#include "bit/pipeline/graphics_pipeline_builder.hpp"
#include "glm/detail/func_packing_simd.inl"
#include <iostream>


PFN_vkCreateDebugUtilsMessengerEXT pfn_vk_create_debug_utils_messenger_ext;
PFN_vkDestroyDebugUtilsMessengerEXT pfn_vk_destroy_debug_utils_messenger_ext;

VKAPI_ATTR VkResult VKAPI_CALL vkCreateDebugUtilsMessengerEXT(VkInstance instance,
                                                              const
                                                              VkDebugUtilsMessengerCreateInfoEXT *
                                                              pCreateInfo,
                                                              const VkAllocationCallbacks *
                                                              pAllocator,
                                                              VkDebugUtilsMessengerEXT *
                                                              pMessenger) {
    return pfn_vk_create_debug_utils_messenger_ext(instance, pCreateInfo, pAllocator, pMessenger);
}

VKAPI_ATTR auto VKAPI_CALL vkDestroyDebugUtilsMessengerEXT(VkInstance instance,
                                                           VkDebugUtilsMessengerEXT messenger,
                                                           VkAllocationCallbacks const *
                                                           pAllocator) -> void {
    return pfn_vk_destroy_debug_utils_messenger_ext(instance, messenger, pAllocator);
}

constexpr uint32_t init_width = 1920;
constexpr uint32_t init_height = 1080;

const std::vector<const char *> required_layers{
#ifndef NDEBUG
    "VK_LAYER_KHRONOS_validation"
#endif
};
const std::vector device_extensions = {
    VK_KHR_SWAPCHAIN_EXTENSION_NAME,
    VK_KHR_TIMELINE_SEMAPHORE_EXTENSION_NAME
};


static std::vector<const char *> get_required_extensions() {
    uint32_t glfwExtensionCount = 0;
    const char **glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

    std::vector extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);
#ifndef NDEBUG
    extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
#endif
    return extensions;
}


static VKAPI_ATTR vk::Bool32 VKAPI_CALL
debug_callback(const vk::DebugUtilsMessageSeverityFlagBitsEXT severity,
               const vk::DebugUtilsMessageTypeFlagsEXT types,
               const vk::DebugUtilsMessengerCallbackDataEXT *pCallbackData,
               void *) {
    std::string message;

    message += vk::to_string(severity) + ": " + vk::to_string(types) + ":\n";
    message += std::string("\t") + "messageIDName   = <" + pCallbackData->pMessageIdName
        + ">\n";
    message += std::string("\t")
        + "messageIdNumber = " + std::to_string(pCallbackData->messageIdNumber) + "\n";
    message += std::string("\t") + "message         = <" + pCallbackData->pMessage + ">\n";
    if (0 < pCallbackData->queueLabelCount) {
        message += std::string("\t") + "Queue Labels:\n";
        for (uint32_t i = 0; i < pCallbackData->queueLabelCount; i++) {
            message += std::string("\t\t") + "labelName = <"
                + pCallbackData->pQueueLabels[i].pLabelName + ">\n";
        }
    }
    if (0 < pCallbackData->cmdBufLabelCount) {
        message += std::string("\t") + "CommandBuffer Labels:\n";
        for (uint32_t i = 0; i < pCallbackData->cmdBufLabelCount; i++) {
            message += std::string("\t\t") + "labelName = <"
                + pCallbackData->pCmdBufLabels[i].pLabelName + ">\n";
        }
    }
    if (0 < pCallbackData->objectCount) {
        for (uint32_t i = 0; i < pCallbackData->objectCount; i++) {
            message += std::string("\t") + "Object " + std::to_string(i) + "\n";
            message += std::string("\t\t") + "objectType   = "
                + vk::to_string(pCallbackData->pObjects[i].objectType) + "\n";
            message += std::string("\t\t") + "objectHandle = "
                + std::to_string(pCallbackData->pObjects[i].objectHandle) + "\n";
            if (pCallbackData->pObjects[i].pObjectName) {
                message += std::string("\t\t") + "objectName   = <"
                    + pCallbackData->pObjects[i].pObjectName + ">\n";
            }
        }
    }

    spdlog::debug(message);
    return false;
}

constexpr int frames_in_flight = 3;


struct rectangle_t {
    uint32_t x, y;
};

class Application {

public:
    Application() {
        this->init_window();
        this->init_vulkan();
        this->create_game_objects();
    }

    void run() {
        int in_flight_frame = 0;
        while (!glfwWindowShouldClose(window)) {
            glfwPollEvents();

            /*if (glfwGetWindowAttrib(window, GLFW_ICONIFIED) != 0) {
                ImGui_ImplGlfw_Sleep(10);
                return;
            }*/

            draw_frame(in_flight_frame);
            in_flight_frame = ++in_flight_frame % frames_in_flight;
        }
    }

    ~Application() {
        spdlog::info("cleaning up");
        this->device->waitIdle();
    }

private:
    static void framebuffer_resize_callback(GLFWwindow *window, int _, int __) {
        const auto app = static_cast<Application *>(glfwGetWindowUserPointer(window));
        app->framebuffer_resized = true;
    }

    void init_window() {
        glfwInit();

        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

        this->window = glfwCreateWindow(init_width, init_height, "BuildIT", nullptr, nullptr);
        glfwSetErrorCallback([](int code, const char *description) {
            spdlog::error("GLFW Error: {} ({})", code, description);
        });
        glfwSetWindowUserPointer(this->window, this);
        glfwSetFramebufferSizeCallback(window, framebuffer_resize_callback);
        spdlog::debug("created glfw window");
    }

    void init_vulkan() {
        this->create_instance();
#ifndef NDEBUG
        this->setup_debug_messenger();
#endif
        this->create_surface();
        this->init_physical_device();
        this->init_device();
        this->init_vma();
        this->create_swapchain();
        this->create_image_views();
        this->create_render_pass();
        this->create_frame_buffers();

        this->create_sync_objects();
        for (int i = 0; i < frames_in_flight; ++i) {
            this->transfer_finished_semaphores.push_back(
                this->device->createSemaphoreUnique({}));
            this->compute_finished_semaphores.push_back(
                this->device->createSemaphoreUnique({}));
        }
        this->create_command_buffers();
    }

    void create_instance() {
        const std::vector<vk::LayerProperties> available_layers
            = vk::enumerateInstanceLayerProperties();
        const bool layers_available = std::ranges::all_of(required_layers,
                                                          [&available_layers](const char *name) {
                                                              if (std::ranges::any_of(
                                                                  available_layers,
                                                                  [&name](
                                                                  vk::LayerProperties const &
                                                                  properties) {
                                                                      return strcmp(name,
                                                                              properties.layerName)
                                                                          ==
                                                                          0;
                                                                  })) {
                                                                  return true;
                                                              }
                                                              spdlog::error(
                                                                  "Validation Layer {} is not available",
                                                                  name);
                                                              return false;
                                                          });
        if (!layers_available) {
            throw std::runtime_error("required layers not available");
        }

        constexpr vk::ApplicationInfo application_info("BuildIT",
                                                       VK_MAKE_API_VERSION(0, 1, 0, 0),
                                                       "BuildIT-Engine",
                                                       VK_MAKE_API_VERSION(0, 1, 0, 0),
                                                       VK_API_VERSION_1_2);

        vk::InstanceCreateInfo create_info({}, &application_info);
        create_info.enabledLayerCount = required_layers.size();
        create_info.ppEnabledLayerNames = required_layers.data();

        const auto extensions = get_required_extensions();
        create_info.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
        create_info.ppEnabledExtensionNames = extensions.data();

        this->instance = vk::createInstanceUnique(create_info);
        spdlog::debug("created vulkan instance");
    }

    void setup_debug_messenger() {
        pfn_vk_create_debug_utils_messenger_ext = reinterpret_cast<
            PFN_vkCreateDebugUtilsMessengerEXT>(
            this->instance->getProcAddr("vkCreateDebugUtilsMessengerEXT"));
        if (pfn_vk_create_debug_utils_messenger_ext == nullptr) {
            throw std::runtime_error("vkCreateDebugUtilsMessengerEXT not found");
        }
        pfn_vk_destroy_debug_utils_messenger_ext = reinterpret_cast<
            PFN_vkDestroyDebugUtilsMessengerEXT>(
            this->instance->getProcAddr("vkDestroyDebugUtilsMessengerEXT"));
        if (pfn_vk_destroy_debug_utils_messenger_ext == nullptr) {
            throw std::runtime_error("vkDestroyDebugUtilsMessengerEXT not found");
        }
        bit::init_debug_names(*this->instance);
        constexpr vk::DebugUtilsMessengerCreateInfoEXT
            create_info({},
                        vk::DebugUtilsMessageSeverityFlagBitsEXT::eError
                        | vk::DebugUtilsMessageSeverityFlagBitsEXT::eWarning,
                        vk::DebugUtilsMessageTypeFlagBitsEXT::eGeneral
                        | vk::DebugUtilsMessageTypeFlagBitsEXT::ePerformance
                        | vk::DebugUtilsMessageTypeFlagBitsEXT::eValidation,
                        debug_callback);

        this->debug_messenger = this->instance->createDebugUtilsMessengerEXTUnique(create_info);
        spdlog::debug("initialized debug messenger");
    }

    void create_surface() {
        VkSurfaceKHR _surface;
        if (glfwCreateWindowSurface(*this->instance, this->window, nullptr, &_surface)
            != VK_SUCCESS) {
            throw std::runtime_error("failed to create window surface");
        }
        const vk::detail::ObjectDestroy<vk::Instance, VULKAN_HPP_DEFAULT_DISPATCHER_TYPE> _deleter(
            this->instance.get()
            );
        this->surface = vk::UniqueSurfaceKHR(vk::SurfaceKHR(_surface), _deleter);
        spdlog::debug("created surface");
    }

    void init_physical_device() {
        const std::vector<vk::PhysicalDevice> devices = this->instance->enumeratePhysicalDevices();
        std::multimap<int, vk::PhysicalDevice> candidates;
        for (const auto &device_candidate : devices) {
            int score = 0;
            const vk::PhysicalDeviceProperties props = device_candidate.getProperties();
            if (props.deviceType == vk::PhysicalDeviceType::eDiscreteGpu) {
                score += 1000;
            }
            candidates.emplace(score, device_candidate);
        }

        this->physical_device = candidates.rbegin()->second;
        const auto deviceProperties = this->physical_device.getProperties();
        const std::string deviceName = deviceProperties.deviceName;
        spdlog::info("picked physical device: " + deviceName);
    }

    void init_device() {
        this->find_queue_family_indices();
        const std::set unique_queue_families = {this->queue_family_indices.graphics_family,
                                                this->queue_family_indices.present_family,
                                                this->queue_family_indices.compute_family,
                                                this->queue_family_indices.transfer_family};

        std::vector<vk::DeviceQueueCreateInfo> queue_create_infos;
        constexpr float queue_priority = 1.0f;
        for (uint32_t queue_family : unique_queue_families) {
            queue_create_infos.emplace_back(vk::DeviceQueueCreateFlags(),
                                            queue_family,
                                            1,
                                            &queue_priority);
        }
        vk::PhysicalDeviceFeatures device_features;
        vk::PhysicalDeviceTimelineSemaphoreFeatures timeline_feature_enable{true};
        const vk::DeviceCreateInfo device_create_info{
            {}, queue_create_infos, required_layers,
            device_extensions, &device_features, &timeline_feature_enable
        };
        this->device = this->physical_device.createDeviceUnique(device_create_info);
        this->graphics_queue = this->device->
            getQueue(this->queue_family_indices.graphics_family, 0);
        this->compute_queue = this->device->getQueue(this->queue_family_indices.compute_family, 0);
        this->present_queue = this->device->getQueue(this->queue_family_indices.present_family, 0);
        this->transfer_queue = this->device->
            getQueue(this->queue_family_indices.transfer_family, 0);
        spdlog::debug("created virtual device");

        bit::set_name(*this->device, this->graphics_queue, "graphics_queue");
        bit::set_name(*this->device, this->compute_queue, "compute_queue");
        bit::set_name(*this->device, this->present_queue, "present_queue");
        bit::set_name(*this->device, this->transfer_queue, "transfer_queue");
    }

    struct find_family_data {
        vk::QueueFamilyProperties properties;
        uint32_t index;
        bool graphics;
        bool compute;
        bool present;
    };

    void find_queue_family_indices() {
        this->queue_family_indices.compute_family = bit::find_simplest_queue_family(
            this->physical_device,
            vk::QueueFlagBits::eCompute);
        this->queue_family_indices.transfer_family = bit::find_simplest_queue_family(
            this->physical_device,
            vk::QueueFlagBits::eTransfer);
        this->queue_family_indices.graphics_family = bit::find_graphics_family(
            this->physical_device,
            *this->surface);
        if (!this->physical_device.getSurfaceSupportKHR(this->queue_family_indices.graphics_family,
                                                        *this->surface)) {
            throw std::runtime_error("failed to get present queue family");
        }
        this->queue_family_indices.present_family = this->queue_family_indices.graphics_family;

        spdlog::info("queue families:");
        spdlog::info("Graphics: {}", this->queue_family_indices.graphics_family);
        spdlog::info("Compute: {}", this->queue_family_indices.compute_family);
        spdlog::info("Preset: {}", this->queue_family_indices.present_family);
        spdlog::info("Transfer: {}", this->queue_family_indices.transfer_family);

        spdlog::debug("found optimal queue family indices");
    }

    void init_vma() {
        VmaVulkanFunctions vulkan_functions = {};
        vulkan_functions.vkGetInstanceProcAddr = &vkGetInstanceProcAddr;
        vulkan_functions.vkGetDeviceProcAddr = &vkGetDeviceProcAddr;
        VmaAllocatorCreateInfo allocator_create_info = {};
        allocator_create_info.flags = VMA_ALLOCATOR_CREATE_EXT_MEMORY_BUDGET_BIT;
        allocator_create_info.vulkanApiVersion = VK_API_VERSION_1_2;
        allocator_create_info.physicalDevice = this->physical_device;
        allocator_create_info.device = *this->device;
        allocator_create_info.instance = *this->instance;
        allocator_create_info.pVulkanFunctions = &vulkan_functions;

        VmaAllocator vma_allocator;
        vmaCreateAllocator(&allocator_create_info, &vma_allocator);

        this->mem_allocator = std::make_unique<bit::memory_allocator>(*this->device, vma_allocator);
        spdlog::debug("initialized vma allocator");
    }

    void create_swapchain() {
        const auto capabilities = this->physical_device.getSurfaceCapabilitiesKHR(*this->surface);
        const auto formats = this->physical_device.getSurfaceFormatsKHR(*this->surface);
        const auto present_modes = this->physical_device.getSurfacePresentModesKHR(*this->surface);

        vk::SurfaceFormatKHR surface_format = formats[0];
        for (const auto &availableFormat : formats) {
            if (availableFormat.format == vk::Format::eB8G8R8A8Srgb
                && availableFormat.colorSpace == vk::ColorSpaceKHR::eSrgbNonlinear) {
                surface_format = availableFormat;
            }
        }

        // Ignore physical device capabilities;
        constexpr auto present_mode = vk::PresentModeKHR::eFifo;
        spdlog::debug("surface capabilities: {} : {}",
                      capabilities.minImageCount,
                      capabilities.maxImageCount);

        this->extent = capabilities.currentExtent;
        if (capabilities.currentExtent.width == std::numeric_limits<uint32_t>::max()) {
            int width, height;
            glfwGetFramebufferSize(window, &width, &height);
            const vk::Extent2D actualExtent = {static_cast<uint32_t>(width),
                                               static_cast<uint32_t>(height)};
            this->extent.width = std::clamp(actualExtent.width,
                                            capabilities.minImageExtent.width,
                                            capabilities.maxImageExtent.width);
            this->extent.height = std::clamp(actualExtent.height,
                                             capabilities.minImageExtent.height,
                                             capabilities.maxImageExtent.height);
        }

        constexpr int image_count = 3;
        vk::SharingMode sharing_mode;
        std::vector<uint32_t> swapchain_queue_family_indices;
        if (this->queue_family_indices.graphics_family != this->queue_family_indices.
            present_family) {
            sharing_mode = vk::SharingMode::eConcurrent;
            swapchain_queue_family_indices = {this->queue_family_indices.graphics_family,
                                              this->queue_family_indices.present_family};
        } else {
            sharing_mode = vk::SharingMode::eExclusive;
            swapchain_queue_family_indices = {this->queue_family_indices.graphics_family};
        }
        const vk::SwapchainCreateInfoKHR swap_chain_create_info({},
                                                                *this->surface,
                                                                image_count,
                                                                surface_format.format,
                                                                surface_format.colorSpace,
                                                                this->extent,
                                                                1,
                                                                vk::ImageUsageFlagBits::eColorAttachment,
                                                                sharing_mode,
                                                                swapchain_queue_family_indices,
                                                                capabilities.currentTransform,
                                                                vk::CompositeAlphaFlagBitsKHR::eOpaque,
                                                                present_mode,
                                                                true,
                                                                nullptr);
        this->swapchain = this->device->createSwapchainKHRUnique(swap_chain_create_info);
        this->swapchain_images = this->device->getSwapchainImagesKHR(*this->swapchain);
        this->swapchain_image_format = surface_format.format;
        this->swapchain_extent = this->extent;
        spdlog::debug("created swapchain");
    }

    void create_image_views() {
        vk::ImageViewCreateInfo image_view_create_info = {
            {}, {}, vk::ImageViewType::e2D, this->swapchain_image_format, {},
            {vk::ImageAspectFlagBits::eColor, 0, 1, 0, 1}
        };
        for (const auto &swapchain_image : this->swapchain_images) {
            image_view_create_info.image = swapchain_image;
            this->swapchain_image_views.push_back(
                this->device->createImageViewUnique(image_view_create_info));
        }
        spdlog::debug("created image views");
    }

    void create_render_pass() {
        vk::AttachmentDescription color_attachment{
            {},
            this->swapchain_image_format,
            vk::SampleCountFlagBits::e1,
            vk::AttachmentLoadOp::eClear,
            vk::AttachmentStoreOp::eStore,
            vk::AttachmentLoadOp::eDontCare,
            vk::AttachmentStoreOp::eDontCare,
            vk::ImageLayout::eUndefined,
            vk::ImageLayout::ePresentSrcKHR
        };
        vk::AttachmentReference color_attachment_ref{0, vk::ImageLayout::eColorAttachmentOptimal};
        vk::SubpassDescription subpass{
            {},
            vk::PipelineBindPoint::eGraphics,
            nullptr,
            color_attachment_ref
        };
        vk::SubpassDependency dependency{
            vk::SubpassExternal,
            0,
            vk::PipelineStageFlagBits::eColorAttachmentOutput,
            vk::PipelineStageFlagBits::eColorAttachmentOutput,
            vk::AccessFlagBits::eNone,
            vk::AccessFlagBits::eColorAttachmentWrite
        };
        const vk::RenderPassCreateInfo render_pass_create_info{
            {},
            color_attachment,
            subpass,
            dependency
        };
        this->render_pass = this->device->createRenderPassUnique(render_pass_create_info);
        spdlog::debug("created renderpass");
    }

    void create_frame_buffers() {
        for (const auto &swapchain_image_view : this->swapchain_image_views) {
            vk::ImageView attachments[] = {*swapchain_image_view};
            vk::FramebufferCreateInfo framebuffer_create_info{
                {},
                *this->render_pass,
                attachments,
                this->swapchain_extent.width,
                this->swapchain_extent.height,
                1
            };
            this->swapchain_framebuffers.push_back(
                this->device->createFramebufferUnique(framebuffer_create_info));
        }
        spdlog::debug("created framebuffers");
    }

    void recreate_swapchain() {
        spdlog::info("recreating swapchain");
        int width = 0, height = 0;
        glfwGetFramebufferSize(window, &width, &height);
        while (width == 0 || height == 0) {
            glfwGetFramebufferSize(window, &width, &height);
            glfwWaitEvents();
        }

        std::vector<vk::Fence> frame_fences{};
        std::ranges::transform(this->in_flight_fences,
                               std::back_inserter(frame_fences),
                               [](const auto &f) {
                                   return *f;
                               });
        if (const auto result = this->device->waitForFences(frame_fences, true, 1000000000);
            result != vk::Result::eSuccess) {
            throw std::runtime_error("waiting for frame fences failed while recreating swapchain");
        }
        this->present_queue.waitIdle();

        this->swapchain_framebuffers.clear();
        this->swapchain_image_views.clear();
        this->swapchain.reset();

        this->image_available_semaphores.clear();
        this->image_ready_present_semaphores.clear();
        this->in_flight_fences.clear();

        this->create_swapchain();
        this->create_image_views();
        this->create_render_pass();
        this->create_frame_buffers();
        this->create_sync_objects();
    }

    void create_sync_objects() {
        for (int i = 0; i < frames_in_flight; ++i) {
            this->image_available_semaphores.push_back(this->device->createSemaphoreUnique({}));
            this->in_flight_fences.push_back(
                this->device->createFenceUnique({vk::FenceCreateFlagBits::eSignaled}));
            bit::set_name(*this->device,
                          *this->image_available_semaphores.back(),
                          "image-available-semaphore-" + std::to_string(i));
            bit::set_name(*this->device,
                          *this->in_flight_fences.back(),
                          "in-flight-fence-" + std::to_string(i));
        }
        for (int i = 0; i < this->swapchain_images.size(); ++i) {
            this->image_ready_present_semaphores.push_back(this->device->createSemaphoreUnique({}));
            bit::set_name(*this->device,
                          *this->image_ready_present_semaphores.back(),
                          "image-ready-present-semaphore-" + std::to_string(i));
        }
        spdlog::info("created sync objects");
    }

    void create_command_buffers() {
        this->command_pool = this->device->createCommandPoolUnique({
            vk::CommandPoolCreateFlagBits::eResetCommandBuffer,
            this->queue_family_indices.graphics_family
        });

        const vk::CommandBufferAllocateInfo alloc_info{
            *this->command_pool,
            vk::CommandBufferLevel::ePrimary,
            frames_in_flight
        };
        this->command_buffers = this->device->allocateCommandBuffersUnique(alloc_info);
        for (int i = 0; i < this->command_buffers.size(); ++i) {
            bit::set_name(*this->device,
                          *this->command_buffers[i],
                          "command-buffer-" + std::to_string(i));
        }
        spdlog::info("created command buffers");
    }

    void create_game_objects() {
        this->culling_descriptor_set_layout = descriptor_set_layout_builder_t{*this->device}
            .add_binding(0,
                         vk::DescriptorType::eStorageBuffer,
                         1,
                         vk::ShaderStageFlagBits::eCompute)
            .add_binding(1,
                         vk::DescriptorType::eStorageBuffer,
                         1,
                         vk::ShaderStageFlagBits::eCompute | vk::ShaderStageFlagBits::eVertex)
            .add_binding(2,
                         vk::DescriptorType::eStorageBuffer,
                         1,
                         vk::ShaderStageFlagBits::eCompute)
            .build();
        this->chip_graphics_descriptor_set_layout = descriptor_set_layout_builder_t{*this->device}
            .add_binding(0,
                         vk::DescriptorType::eStorageBuffer,
                         1,
                         vk::ShaderStageFlagBits::eVertex)
            .add_binding(1,
                         vk::DescriptorType::eStorageBuffer,
                         1,
                         vk::ShaderStageFlagBits::eVertex)
            .build();

        // Pipelines

        constexpr vk::PushConstantRange push_constant_range{
            vk::ShaderStageFlagBits::eCompute, 0, 4 * sizeof(float)
        };
        vk::UniqueShaderModule module = bit::load_shader_unique(*this->device, "culling.comp.spv");
        this->culling_pipeline = bit::compute_pipeline_builder{*this->device}
            .add_descriptor_set_layout(*this->culling_descriptor_set_layout)
            .add_push_constant_range(push_constant_range)
            .set_shader_module(module).build();
        bit::set_name(*this->device, *this->culling_pipeline.handle, "compute-pipeline");
        bit::set_name(*this->device, *this->culling_pipeline.layout, "compute-pipeline-layout");

        vk::UniqueShaderModule reset_module = bit::load_shader_unique(
            *this->device,
            "reset-culling.comp.spv");
        this->reset_culling_pipeline = bit::compute_pipeline_builder{*this->device}
            .add_descriptor_set_layout(*this->culling_descriptor_set_layout)
            .add_push_constant_range(push_constant_range)
            .set_shader_module(reset_module).build();
        bit::set_name(*this->device,
                      *this->reset_culling_pipeline.handle,
                      "reset-compute-pipeline");
        bit::set_name(*this->device,
                      *this->reset_culling_pipeline.layout,
                      "reset-compute-pipeline-layout");

        auto dynamic_states = std::vector{vk::DynamicState::eViewport, vk::DynamicState::eScissor};
        vk::Viewport viewport{};
        vk::Rect2D scissor{};
        vk::PipelineColorBlendAttachmentState color_blend_attachment(false,
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
        const vk::PipelineColorBlendStateCreateInfo
            color_blending(vk::PipelineColorBlendStateCreateFlags(),
                           false,
                           vk::LogicOp::eCopy,
                           color_blend_attachment,
                           {0.0, 0.0, 0.0, 0.0});
        vk::UniqueShaderModule vert_shader = bit::load_shader_unique(
            *this->device,
            "instanced-rect.vert.spv");
        vk::UniqueShaderModule frag_shader = bit::load_shader_unique(
            *this->device,
            "instanced-rect.frag.spv");
        this->chip_graphics_pipeline = bit::graphics_pipeline_builder(
                *this->device,
                *this->render_pass)
            .add_shader_stage(vk::ShaderStageFlagBits::eVertex, *vert_shader)
            .add_shader_stage(vk::ShaderStageFlagBits::eFragment, *frag_shader)
            .add_descriptor_set_layout(*this->culling_descriptor_set_layout)
            .add_descriptor_set_layout(*this->chip_graphics_descriptor_set_layout)
            .add_push_constant_range(
                vk::PushConstantRange{vk::ShaderStageFlagBits::eVertex, 0, 3 * 3 * sizeof(float)})
            .set_vertex_input_state({vk::PipelineVertexInputStateCreateFlags(), nullptr, nullptr})
            .set_input_assembly_state({vk::PipelineInputAssemblyStateCreateFlags(),
                                       vk::PrimitiveTopology::eTriangleList,
                                       false})
            .set_dynamic_state({vk::PipelineDynamicStateCreateFlags(), dynamic_states})
            .set_viewport_state({vk::PipelineViewportStateCreateFlags(), viewport, scissor})
            .set_rasterization_state({vk::PipelineRasterizationStateCreateFlags(),
                                      false,
                                      false,
                                      vk::PolygonMode::eFill,
                                      vk::CullModeFlagBits::eBack,
                                      vk::FrontFace::eClockwise,
                                      false,
                                      0.0f,
                                      0.0f,
                                      0.0f,
                                      1.0f})
            .set_color_blend_state(color_blending)
            .build();
        bit::set_name(*this->device,
                      *this->chip_graphics_pipeline.handle,
                      "chip-graphics-pipeline");
        bit::set_name(*this->device,
                      *this->chip_graphics_pipeline.layout,
                      "chip-graphics-pipeline-layout");

        // Command and Descriptor Pools

        std::vector compute_pool_sizes{
            vk::DescriptorPoolSize{vk::DescriptorType::eStorageBuffer, frames_in_flight * 3}
        };
        this->compute_descriptor_pool = this->device->createDescriptorPoolUnique({
            {}, frames_in_flight, compute_pool_sizes});
        std::vector graphics_pool_sizes{
            vk::DescriptorPoolSize{vk::DescriptorType::eStorageBuffer, frames_in_flight * 2}
        };
        this->graphics_descriptor_pool = this->device->createDescriptorPoolUnique({
            {}, frames_in_flight, graphics_pool_sizes});

        this->compute_command_pool = this->device->createCommandPoolUnique({
            vk::CommandPoolCreateFlagBits::eResetCommandBuffer,
            this->queue_family_indices.compute_family});
        this->compute_command_buffers = this->device->allocateCommandBuffersUnique(
            {*this->compute_command_pool, vk::CommandBufferLevel::ePrimary, frames_in_flight});

        this->transfer_command_pool = this->device->createCommandPoolUnique({
            vk::CommandPoolCreateFlagBits::eResetCommandBuffer,
            this->queue_family_indices.transfer_family
        });
        this->transfer_command_buffers = this->device->allocateCommandBuffersUnique(
            {*this->transfer_command_pool, vk::CommandBufferLevel::ePrimary, frames_in_flight});

        this->graphics_command_pool = this->device->createCommandPoolUnique({
            vk::CommandPoolCreateFlagBits::eResetCommandBuffer,
            this->queue_family_indices.graphics_family});
        this->graphics_command_buffers = this->device->allocateCommandBuffersUnique(
            {*this->graphics_command_pool, vk::CommandBufferLevel::ePrimary, frames_in_flight});

        for (int i = 0; i < frames_in_flight; ++i) {
            bit::set_name(*this->device,
                          *this->compute_command_buffers[i],
                          "compute-command-buffer-" + std::to_string(i));
            bit::set_name(*this->device,
                          *this->transfer_command_buffers[i],
                          "transfer-command-buffer-" + std::to_string(i));
            bit::set_name(*this->device,
                          *this->graphics_command_buffers[i],
                          "graphics-command-buffer-" + std::to_string(i));
        }

        // Test Chip Buffer

        this->test_chip_buffer = std::make_unique<chip_buffer<rectangle_t> >(
            *this->device,
            this->queue_family_indices,
            *this->mem_allocator,
            this->culling_pipeline,
            this->reset_culling_pipeline,
            this->chip_graphics_pipeline,
            *this->culling_descriptor_set_layout,
            *this->chip_graphics_descriptor_set_layout,
            *this->compute_descriptor_pool,
            *this->graphics_descriptor_pool,
            this->registry,
            this->extent);
    }

    void draw_frame(const int in_flight_frame) {
        if (this->device->waitForFences(*this->in_flight_fences[in_flight_frame],
                                        vk::True,
                                        1000000000) != vk::Result::eSuccess) {
            throw std::runtime_error("failed to wait for in flight fence");
        }
        const auto next_image = this->device->acquireNextImageKHR(
            *this->swapchain,
            UINT64_MAX,
            *this->image_available_semaphores[in_flight_frame],
            nullptr);
        if (next_image.result == vk::Result::eErrorOutOfDateKHR ||
            next_image.result == vk::Result::eSuboptimalKHR ||
            this->framebuffer_resized) {
            spdlog::info("Image suboptimal or out of date");
            this->recreate_swapchain();
            this->framebuffer_resized = false;
            return;
        }
        // Reset fence after potential swapchain recreation to avoid deadlock
        this->device->resetFences(*this->in_flight_fences[in_flight_frame]);

        if (next_image.result != vk::Result::eSuccess) {
            throw std::runtime_error("failed to acquire next image");
        }
        const unsigned image_index = next_image.value;

        this->transfer_command_buffers[in_flight_frame]->reset();
        this->compute_command_buffers[in_flight_frame]->reset();
        this->command_buffers[in_flight_frame]->reset();

        const vk::CommandBuffer &command_buffer = *this->command_buffers[in_flight_frame];
        constexpr vk::CommandBufferBeginInfo beginInfo(
            vk::CommandBufferUsageFlagBits::eOneTimeSubmit,
            nullptr);
        if (command_buffer.begin(&beginInfo) != vk::Result::eSuccess) {
            throw std::runtime_error("failed to begin recording command buffer");
        }

        std::vector clear_values{vk::ClearValue{vk::ClearColorValue{0.0f, 0.0f, 0.0f, 1.0f}}};
        const vk::RenderPassBeginInfo render_pass_info(*this->render_pass,
                                                       *this->swapchain_framebuffers[image_index],
                                                       vk::Rect2D({0, 0}, this->swapchain_extent),
                                                       clear_values);
        command_buffer.beginRenderPass(render_pass_info,
                                       vk::SubpassContents::eInline);
        const vk::Viewport viewport(0.0f,
                                    0.0f,
                                    static_cast<float>(this->swapchain_extent.width),
                                    static_cast<float>(this->swapchain_extent.height),
                                    0.0f,
                                    1.0f);
        command_buffer.setViewport(0, viewport);
        const vk::Rect2D scissor({0, 0}, this->swapchain_extent);
        command_buffer.setScissor(0, scissor);

        this->transfer_command_buffers[in_flight_frame]->begin(
            {vk::CommandBufferUsageFlagBits::eOneTimeSubmit});
        this->compute_command_buffers[in_flight_frame]->begin(
            {vk::CommandBufferUsageFlagBits::eOneTimeSubmit});

        std::vector graphics_wait_semaphores{
            *this->image_available_semaphores[in_flight_frame]};
        std::vector<vk::PipelineStageFlags> graphics_wait_stages{
            vk::PipelineStageFlagBits::eFragmentShader};
        bool recorded = this->test_chip_buffer->record_buffer(
            *this->transfer_command_buffers[in_flight_frame],
            *this->compute_command_buffers[in_flight_frame],
            command_buffer,
            in_flight_frame);
        this->transfer_command_buffers[in_flight_frame]->end();
        this->compute_command_buffers[in_flight_frame]->end();
        if (recorded) {
            // Transfer
            const vk::SubmitInfo transfer_submit_info{
                {}, {}, *this->transfer_command_buffers[in_flight_frame],
                *this->transfer_finished_semaphores[in_flight_frame]};
            this->transfer_queue.submit(transfer_submit_info, nullptr);

            // Culling
            std::vector<vk::PipelineStageFlags> compute_wait_stages{
                vk::PipelineStageFlagBits::eComputeShader};
            const vk::SubmitInfo compute_submit_info{
                *this->transfer_finished_semaphores[in_flight_frame],
                compute_wait_stages,
                *this->compute_command_buffers[in_flight_frame],
                *this->compute_finished_semaphores[in_flight_frame]
            };
            this->compute_queue.submit(compute_submit_info, nullptr);

            graphics_wait_semaphores.push_back(*this->compute_finished_semaphores[in_flight_frame]);
            graphics_wait_stages.emplace_back(vk::PipelineStageFlagBits::eVertexShader);
        }

        command_buffer.endRenderPass();
        command_buffer.end();

        const vk::SubmitInfo graphics_submit_info{
            graphics_wait_semaphores,
            graphics_wait_stages,
            command_buffer,
            *this->image_ready_present_semaphores[image_index]
        };
        this->graphics_queue.submit(graphics_submit_info, *this->in_flight_fences[in_flight_frame]);

        const vk::PresentInfoKHR present_info{
            *this->image_ready_present_semaphores[image_index],
            *this->swapchain,
            image_index,
            nullptr
        };

        try {
            const vk::Result res = this->present_queue.presentKHR(present_info);
            if (res != vk::Result::eSuccess) {
                throw std::runtime_error("failed to present");
            }
        } catch (const vk::SystemError &e) {
            if (e.code() == vk::Result::eErrorOutOfDateKHR
                || e.code() == vk::Result::eSuboptimalKHR) {
                this->recreate_swapchain();
            } else {
                throw std::runtime_error("failed to present");
            }
        }
    }

    GLFWwindow *window = nullptr;
    bool framebuffer_resized = false;
    vk::UniqueInstance instance;
    vk::UniqueDebugUtilsMessengerEXT debug_messenger;
    vk::UniqueSurfaceKHR surface;
    vk::PhysicalDevice physical_device;
    vk::UniqueDevice device;
    bit::queue_family_indices_t queue_family_indices{};
    vk::Queue compute_queue;
    vk::Queue graphics_queue;
    vk::Queue present_queue;
    vk::Queue transfer_queue;
    std::unique_ptr<bit::memory_allocator> mem_allocator = nullptr;
    vk::Extent2D extent;
    vk::UniqueSwapchainKHR swapchain;
    std::vector<vk::Image> swapchain_images;
    vk::Format swapchain_image_format{};
    vk::Extent2D swapchain_extent;
    std::vector<vk::UniqueImageView> swapchain_image_views;
    vk::UniqueRenderPass render_pass;
    std::vector<vk::UniqueFramebuffer> swapchain_framebuffers;

    std::vector<vk::UniqueSemaphore> image_available_semaphores;
    std::vector<vk::UniqueSemaphore> image_ready_present_semaphores;
    std::vector<vk::UniqueFence> in_flight_fences;

    vk::UniqueCommandPool command_pool;
    std::vector<vk::UniqueCommandBuffer> command_buffers;

    // Game Data

    entt::registry registry;
    bit::pipeline culling_pipeline;
    bit::pipeline reset_culling_pipeline;
    bit::pipeline chip_graphics_pipeline;
    vk::UniqueDescriptorSetLayout culling_descriptor_set_layout;
    vk::UniqueDescriptorSetLayout chip_graphics_descriptor_set_layout;
    vk::UniqueCommandPool compute_command_pool;
    std::vector<vk::UniqueCommandBuffer> compute_command_buffers;
    vk::UniqueCommandPool transfer_command_pool;
    std::vector<vk::UniqueCommandBuffer> transfer_command_buffers;
    vk::UniqueCommandPool graphics_command_pool;
    std::vector<vk::UniqueCommandBuffer> graphics_command_buffers;
    vk::UniqueDescriptorPool compute_descriptor_pool;
    vk::UniqueDescriptorPool graphics_descriptor_pool;
    std::unique_ptr<chip_buffer<rectangle_t> > test_chip_buffer = nullptr;
    std::vector<vk::UniqueSemaphore> transfer_finished_semaphores;
    std::vector<vk::UniqueSemaphore> compute_finished_semaphores;

    // Synchronization for data transfer
};

int main() {
    spdlog::set_level(spdlog::level::debug);

    Application app;
    app.run();
    spdlog::info("closing application");

    return EXIT_SUCCESS;
}