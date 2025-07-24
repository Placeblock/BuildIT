#define GLFW_INCLUDE_VULKAN
#include "../../../lib/imgui/backends/imgui_impl_glfw.h"
#include "../../../lib/imgui/backends/imgui_impl_vulkan.h"
#include "../../../lib/imgui/imgui.h"
#include "app/vulkan/circuitboard_manager.hpp"
#include "app/vulkan/imgui_circuitboard.hpp"
#include "imgui_internal.h"
#include <GLFW/glfw3.h>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <map>
#include <set>
#include <spdlog/spdlog.h>
#include <vulkan/vulkan.hpp>

constexpr uint32_t WIDTH = 800;
constexpr uint32_t HEIGHT = 600;

const std::vector VALIDATION_LAYERS = {"VK_LAYER_KHRONOS_validation"};
const std::vector DEVICE_EXTENSIONS = {VK_KHR_SWAPCHAIN_EXTENSION_NAME};

#ifdef NDEBUG
constexpr bool enableValidationLayers = false;
#else
constexpr bool enableValidationLayers = true;
#endif

constexpr uint32_t MAX_FRAMES_IN_FLIGHT = 3;

PFN_vkCreateDebugUtilsMessengerEXT pfnVkCreateDebugUtilsMessengerEXT;
PFN_vkDestroyDebugUtilsMessengerEXT pfnVkDestroyDebugUtilsMessengerEXT;

VKAPI_ATTR VkResult VKAPI_CALL
vkCreateDebugUtilsMessengerEXT(VkInstance instance,
                               const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo,
                               const VkAllocationCallbacks* pAllocator,
                               VkDebugUtilsMessengerEXT* pMessenger) {
    return pfnVkCreateDebugUtilsMessengerEXT(instance, pCreateInfo, pAllocator, pMessenger);
}

VKAPI_ATTR void VKAPI_CALL vkDestroyDebugUtilsMessengerEXT(VkInstance instance,
                                                           VkDebugUtilsMessengerEXT messenger,
                                                           VkAllocationCallbacks const* pAllocator) {
    return pfnVkDestroyDebugUtilsMessengerEXT(instance, messenger, pAllocator);
}

struct swap_chain_support_details {
    vk::SurfaceCapabilitiesKHR capabilities;
    std::vector<vk::SurfaceFormatKHR> formats;
    std::vector<vk::PresentModeKHR> present_modes;
};

class HelloTriangleApplication {
public:
    void run() {
        initWindow();
        initVulkan();
        initImGUI();
        this->create_circuit_boards();
        std::thread fps([=] { measure(); });
        fps.detach();
        mainLoop();
        cleanup();
    }

    ~HelloTriangleApplication() {
        std::cout << "Cleaning up" << std::endl;
    }

private:
    void initWindow() {
        glfwInit();

        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        //glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

        window = glfwCreateWindow(WIDTH, HEIGHT, "Vulkan", nullptr, nullptr);
        glfwSetFramebufferSizeCallback(window, framebufferResizeCallback);
        glfwSetWindowUserPointer(window, this);
        glfwSetFramebufferSizeCallback(window, framebufferResizeCallback);
    }

    static void framebufferResizeCallback(GLFWwindow* window, int width, int height) {
        std::cout << "RESIZE!" << std::endl;
        const auto app = static_cast<HelloTriangleApplication*>(glfwGetWindowUserPointer(window));
        app->framebufferResized = true;
    }

    void initVulkan() {
        this->createInstance();
        this->setupDebugMessenger();
        this->createSurface();
        this->pickPhsicalDevice();
        this->createLogicalDevice();
        this->createSwapChain();
        this->createImageViews();
        this->createRenderPass();
        //this->createGraphicsPipeline();
        this->createFrameBuffers();
        this->createCommandPool();
        this->createCommandBuffers();
        this->createSyncObjects();
    }

    void initImGUI() {
        std::vector poolSizes = {
            vk::DescriptorPoolSize{vk::DescriptorType::eCombinedImageSampler, 64}};
        vk::DescriptorPoolCreateInfo poolInfo
            = {vk::DescriptorPoolCreateFlagBits::eFreeDescriptorSet, 0, poolSizes};
        for (const auto vk_descriptor_pool_size : poolSizes) {
            poolInfo.maxSets += vk_descriptor_pool_size.descriptorCount;
        }
        this->imguiDescriptorPool = this->ctx->device.createDescriptorPool(poolInfo);

        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO();
        ImFont* font = io.Fonts
                           ->AddFontFromFileTTF("../../../assets/Roboto-VariableFont_wdth,wght.ttf",
                                                20,
                                                nullptr,
                                                io.Fonts->GetGlyphRangesDefault());
        io.FontDefault = font;
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
        ImGui::StyleColorsDark();

        ImGui_ImplGlfw_InitForVulkan(this->window, true);

        ImGui_ImplVulkan_InitInfo vulkanInitInfo = {};
        vulkanInitInfo.Instance = this->instance;
        vulkanInitInfo.PhysicalDevice = this->ctx->physical_device;
        vulkanInitInfo.Device = this->ctx->device;
        vulkanInitInfo.Queue = this->graphicsQueue;
        vulkanInitInfo.DescriptorPool = this->imguiDescriptorPool;
        vulkanInitInfo.RenderPass = this->renderPass;
        vulkanInitInfo.Subpass = 0;
        vulkanInitInfo.MinImageCount = 3;
        vulkanInitInfo.ImageCount = 3;
        vulkanInitInfo.MSAASamples = VK_SAMPLE_COUNT_1_BIT;

        ImGui_ImplVulkan_Init(&vulkanInitInfo);
    }

    void createInstance() {
        const std::vector<const char*> required_layers = get_required_layers();
        const std::vector<vk::LayerProperties> available_layers
            = vk::enumerateInstanceLayerProperties();
        if (!check_layers(required_layers, available_layers)) {
            throw std::runtime_error("validation layers not available!");
        }

        const vk::ApplicationInfo applicationInfo("Hello Triangle",
                                                  VK_MAKE_API_VERSION(0, 1, 0, 0),
                                                  "No Engine",
                                                  VK_MAKE_API_VERSION(0, 1, 0, 0),
                                                  VK_API_VERSION_1_0);
        vk::InstanceCreateInfo createInfo({}, &applicationInfo);
        createInfo.enabledLayerCount = required_layers.size();
        createInfo.ppEnabledLayerNames = required_layers.data();

        const auto extensions = get_required_extensions();
        createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
        createInfo.ppEnabledExtensionNames = extensions.data();

        if (const vk::Result res = vk::createInstance(&createInfo, nullptr, &this->instance);
            res != vk::Result::eSuccess) {
            throw std::runtime_error("failed to create instance");
        }
    }

    [[nodiscard]] static bool check_layers(std::vector<const char*> required_layer_names,
                                           std::vector<vk::LayerProperties> available_layers) {
        return std::ranges::all_of(required_layer_names, [&available_layers](const char* name) {
            if (std::ranges::any_of(available_layers,
                                    [&name](vk::LayerProperties const& properties) {
                                        return strcmp(name, properties.layerName) == 0;
                                    })) {
                return true;
            }
            spdlog::error("Validation Layer {} is not available", name);
            return false;
        });
    }

    static std::vector<const char*> get_required_layers() {
        std::vector<const char*> layers;
        if (enableValidationLayers) {
            layers.push_back("VK_LAYER_KHRONOS_validation");
        }
        return layers;
    }

    static std::vector<const char*> get_required_extensions() {
        uint32_t glfwExtensionCount = 0;
        const char** glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

        std::vector extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);
        if (enableValidationLayers) {
            extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
        }
        return extensions;
    }

    static VKAPI_ATTR vk::Bool32 VKAPI_CALL
    debugCallback(vk::DebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
                  vk::DebugUtilsMessageTypeFlagsEXT messageTypes,
                  const vk::DebugUtilsMessengerCallbackDataEXT* pCallbackData,
                  void*) {
        std::string message;

        message += vk::to_string(messageSeverity) + ": " + vk::to_string(messageTypes) + ":\n";
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

        std::cout << message << std::endl;

        return false;
    }

    void setupDebugMessenger() {
        if constexpr (!enableValidationLayers)
            return;

        pfnVkCreateDebugUtilsMessengerEXT = reinterpret_cast<PFN_vkCreateDebugUtilsMessengerEXT>(
            instance.getProcAddr("vkCreateDebugUtilsMessengerEXT"));
        if (pfnVkCreateDebugUtilsMessengerEXT == nullptr) {
            throw std::runtime_error("vkCreateDebugUtilsMessengerEXT not found");
        }
        pfnVkDestroyDebugUtilsMessengerEXT = reinterpret_cast<PFN_vkDestroyDebugUtilsMessengerEXT>(
            instance.getProcAddr("vkDestroyDebugUtilsMessengerEXT"));
        if (pfnVkDestroyDebugUtilsMessengerEXT == nullptr) {
            throw std::runtime_error("vkDestroyDebugUtilsMessengerEXT not found");
        }
        vk::DebugUtilsMessengerCreateInfoEXT
            createInfo({},
                       vk::DebugUtilsMessageSeverityFlagBitsEXT::eError
                           | vk::DebugUtilsMessageSeverityFlagBitsEXT::eWarning,
                       vk::DebugUtilsMessageTypeFlagBitsEXT::eGeneral
                           | vk::DebugUtilsMessageTypeFlagBitsEXT::ePerformance
                           | vk::DebugUtilsMessageTypeFlagBitsEXT::eValidation,
                       debugCallback);

        this->debugMessenger = instance.createDebugUtilsMessengerEXT(createInfo);
    }

    void pickPhsicalDevice() {
        const std::vector<vk::PhysicalDevice> devices = instance.enumeratePhysicalDevices();
        std::multimap<int, vk::PhysicalDevice> candidates;
        for (auto physical_device : devices) {
            int score = this->weightDevice(physical_device);
            candidates.insert(std::make_pair(score, physical_device));
        }
        this->ctx->physical_device = candidates.begin()->second;
    }

    bool isDeviceSuitable(const vk::PhysicalDevice& device) {
        const std::vector<vk::QueueFamilyProperties> properties = device.getQueueFamilyProperties();
        try {
            this->find_queue_families(properties);
        } catch (std::runtime_error& e) {
            return false;
        }
        if (const auto swap_chain_support_details = this->query_swap_chain_support_details(device);
            swap_chain_support_details.formats.empty()
            || swap_chain_support_details.present_modes.empty()) {
            return false;
        }
        return checkDeviceExtensionSupport(device);
    }

    bool checkDeviceExtensionSupport(const vk::PhysicalDevice& device) {
        std::vector<vk::ExtensionProperties> available_extensions;
        std::set<std::string> required_extensions(DEVICE_EXTENSIONS.begin(),
                                                  DEVICE_EXTENSIONS.end());
        for (const auto& extension : available_extensions) {
            required_extensions.erase(extension.extensionName);
        }
        return required_extensions.empty();
    }

    size_t weightDevice(const vk::PhysicalDevice& device) {
        vk::PhysicalDeviceProperties props = device.getProperties();
        vk::PhysicalDeviceFeatures features = device.getFeatures();

        int score = 0;

        if (props.deviceType == vk::PhysicalDeviceType::eDiscreteGpu) {
            score += 1000;
        }
        return score;
    }

    queue_family_indices find_queue_families(
        const std::vector<vk::QueueFamilyProperties>& queue_families) const {
        queue_family_indices indices;
        size_t graphics_family_index = -1;
        for (int i = 0; i < queue_families.size(); ++i) {
            if (queue_families[i].queueFlags & vk::QueueFlagBits::eGraphics) {
                graphics_family_index = i;
                break;
            }
        }
        if (graphics_family_index == -1) {
            throw std::runtime_error("failed to find a graphics family");
        }
        indices.graphics_family = graphics_family_index;
        if (this->ctx->physical_device.getSurfaceSupportKHR(static_cast<uint32_t>(
                                                                graphics_family_index),
                                                            this->surface)) {
            indices.present_family = graphics_family_index;
        } else {
            size_t present_family_index = -1;
            for (int i = 0; i < queue_families.size(); ++i) {
                if (this->ctx->physical_device.getSurfaceSupportKHR(static_cast<uint32_t>(i),
                                                                    this->surface)) {
                    present_family_index = i;
                    break;
                }
            }
            if (present_family_index == -1) {
                throw std::runtime_error("failed to find present surface");
            }
            indices.present_family = present_family_index;
        }
        return indices;
    }

    swap_chain_support_details query_swap_chain_support_details(vk::PhysicalDevice device) {
        swap_chain_support_details details;
        details.capabilities = device.getSurfaceCapabilitiesKHR(surface);
        details.formats = device.getSurfaceFormatsKHR(surface);
        details.present_modes = device.getSurfacePresentModesKHR(surface);
        return details;
    }

    vk::SurfaceFormatKHR choose_swap_surface_format(
        const std::vector<vk::SurfaceFormatKHR>& available_formats) {
        for (const auto& availableFormat : available_formats) {
            if (availableFormat.format == vk::Format::eB8G8R8A8Srgb
                && availableFormat.colorSpace == vk::ColorSpaceKHR::eSrgbNonlinear) {
                return availableFormat;
            }
        }
        return available_formats[0];
    }

    vk::PresentModeKHR choose_swap_present_mode(
        const std::vector<vk::PresentModeKHR>& available_present_modes) {
        return vk::PresentModeKHR::eImmediate;
    }

    vk::Extent2D choose_swap_extent(const vk::SurfaceCapabilitiesKHR& capabilities) {
        if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max()) {
            return capabilities.currentExtent;
        }
        int width, height;
        glfwGetFramebufferSize(window, &width, &height);

        vk::Extent2D actualExtent = {static_cast<uint32_t>(width), static_cast<uint32_t>(height)};

        actualExtent.width = std::clamp(actualExtent.width,
                                        capabilities.minImageExtent.width,
                                        capabilities.maxImageExtent.width);
        actualExtent.height = std::clamp(actualExtent.height,
                                         capabilities.minImageExtent.height,
                                         capabilities.maxImageExtent.height);

        return actualExtent;
    }

    void createLogicalDevice() {
        const std::vector<vk::QueueFamilyProperties> properties = this->ctx->physical_device
                                                                      .getQueueFamilyProperties();
        const auto [graphics_family, present_family] = this->find_queue_families(properties);
        std::set unique_queue_families = {graphics_family, present_family};
        std::vector<vk::DeviceQueueCreateInfo> queue_create_infos;
        constexpr float queuePriority = 1.0f;
        for (uint32_t queue_family : unique_queue_families) {
            queue_create_infos.emplace_back(vk::DeviceQueueCreateFlags(),
                                            queue_family,
                                            1,
                                            &queuePriority);
        }

        constexpr vk::PhysicalDeviceFeatures deviceFeatures{};
        std::vector<const char*> layers = get_required_layers();
        const vk::DeviceCreateInfo deviceCreateInfo(vk::DeviceCreateFlags(),
                                                    queue_create_infos,
                                                    layers,
                                                    DEVICE_EXTENSIONS,
                                                    &deviceFeatures);
        this->ctx->device = this->ctx->physical_device.createDevice(deviceCreateInfo);
        this->graphicsQueue = this->ctx->device.getQueue(graphics_family, 0);
        this->presentQueue = this->ctx->device.getQueue(present_family, 0);
    }

    void createSurface() {
        VkSurfaceKHR _surface;
        if (glfwCreateWindowSurface(this->instance, this->window, nullptr, &_surface)
            != VK_SUCCESS) {
            throw std::runtime_error("failed to create window surface");
        }
        this->surface = vk::SurfaceKHR(_surface);
    }

    void createSwapChain() {
        swap_chain_support_details swap_chain_support = query_swap_chain_support_details(
            this->ctx->physical_device);
        vk::SurfaceFormatKHR surface_format = this->choose_swap_surface_format(
            swap_chain_support.formats);
        vk::PresentModeKHR present_mode = this->choose_swap_present_mode(
            swap_chain_support.present_modes);
        vk::Extent2D extent = this->choose_swap_extent(swap_chain_support.capabilities);

        uint32_t imageCount = swap_chain_support.capabilities.minImageCount + 1;
        std::cout << "Image count: " << imageCount << std::endl;
        if (swap_chain_support.capabilities.maxImageCount > 0
            && imageCount > swap_chain_support.capabilities.maxImageCount) {
            imageCount = swap_chain_support.capabilities.maxImageCount;
        }
        vk::SharingMode sharingMode;
        const std::vector<vk::QueueFamilyProperties> properties = this->ctx->physical_device
                                                                      .getQueueFamilyProperties();
        this->ctx->queue_families = find_queue_families(properties);
        std::vector<uint32_t> queue_family_indices;
        if (this->ctx->queue_families.graphics_family != this->ctx->queue_families.present_family) {
            sharingMode = vk::SharingMode::eConcurrent;
            queue_family_indices = {this->ctx->queue_families.graphics_family,
                                    this->ctx->queue_families.present_family};
        } else {
            sharingMode = vk::SharingMode::eExclusive;
        }
        vk::SwapchainCreateInfoKHR swap_chain_create_info({},
                                                          surface,
                                                          imageCount,
                                                          surface_format.format,
                                                          surface_format.colorSpace,
                                                          extent,
                                                          1,
                                                          vk::ImageUsageFlagBits::eColorAttachment,
                                                          sharingMode,
                                                          queue_family_indices,
                                                          swap_chain_support.capabilities
                                                              .currentTransform,
                                                          vk::CompositeAlphaFlagBitsKHR::eOpaque,
                                                          present_mode,
                                                          true,
                                                          nullptr);
        this->swapChain = this->ctx->device.createSwapchainKHR(swap_chain_create_info);
        this->swapChainImages = this->ctx->device.getSwapchainImagesKHR(this->swapChain);
        this->swapChainImageFormat = surface_format.format;
        this->swapChainExtent = extent;
    }

    void createImageViews() {
        this->swapChainImageViews.reserve(this->swapChainImages.size());
        vk::ImageViewCreateInfo imageViewCreateInfo({},
                                                    {},
                                                    vk::ImageViewType::e2D,
                                                    this->swapChainImageFormat,
                                                    {},
                                                    {vk::ImageAspectFlagBits::eColor, 0, 1, 0, 1});
        for (const auto image : swapChainImages) {
            imageViewCreateInfo.image = image;
            this->swapChainImageViews.push_back(
                this->ctx->device.createImageView(imageViewCreateInfo));
        }
    }

    void createRenderPass() {
        vk::AttachmentDescription colorAttachment(vk::AttachmentDescriptionFlags(),
                                                  this->swapChainImageFormat,
                                                  vk::SampleCountFlagBits::e1,
                                                  vk::AttachmentLoadOp::eDontCare,
                                                  vk::AttachmentStoreOp::eStore,
                                                  vk::AttachmentLoadOp::eDontCare,
                                                  vk::AttachmentStoreOp::eDontCare,
                                                  vk::ImageLayout::eUndefined,
                                                  vk::ImageLayout::ePresentSrcKHR);
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
        this->renderPass = this->ctx->device.createRenderPass(renderPassInfo);
    }

    void createFrameBuffers() {
        this->swapChainFramebuffers.reserve(this->swapChainImages.size());
        for (const auto swapChainImageView : this->swapChainImageViews) {
            vk::ImageView attachments[] = {swapChainImageView};
            vk::FramebufferCreateInfo framebufferInfo(vk::FramebufferCreateFlags(),
                                                      this->renderPass,
                                                      attachments,
                                                      swapChainExtent.width,
                                                      swapChainExtent.height,
                                                      1);
            this->swapChainFramebuffers.emplace_back(
                this->ctx->device.createFramebuffer(framebufferInfo));
        }
    }

    void cleanupSwapChain() {
        for (auto swapChainFramebuffer : this->swapChainFramebuffers) {
            this->ctx->device.destroyFramebuffer(swapChainFramebuffer);
        }
        this->swapChainFramebuffers.clear();
        for (const auto& imageView : this->swapChainImageViews) {
            this->ctx->device.destroyImageView(imageView);
        }
        this->swapChainImageViews.clear();
        this->ctx->device.destroySwapchainKHR(this->swapChain);
    }

    void recreateSwapChain() {
        int width = 0, height = 0;
        glfwGetFramebufferSize(window, &width, &height);
        while (width == 0 || height == 0) {
            glfwGetFramebufferSize(window, &width, &height);
            glfwWaitEvents();
        }

        this->ctx->device.waitIdle();

        this->cleanupSwapChain();

        this->createSwapChain();
        this->createImageViews();
        this->createFrameBuffers();
    }

    void createCommandBuffers() {
        const vk::CommandBufferAllocateInfo allocInfo(this->commandPool,
                                                      vk::CommandBufferLevel::ePrimary,
                                                      this->swapChainImages.size());
        this->commandBuffers = this->ctx->device.allocateCommandBuffers(allocInfo);
    }

    void createCommandPool() {
        this->commandPool = this->ctx->device.createCommandPool(
            {vk::CommandPoolCreateFlagBits::eResetCommandBuffer,
             this->ctx->queue_families.graphics_family});
    }

    void create_circuit_boards() {
        this->board_manager = std::make_unique<circuitboard_manager>(*ctx);
        this->imgui_boards.emplace_back(*this->board_manager->create_board());
        this->imgui_boards.emplace_back(*this->board_manager->create_board());
        this->imgui_boards.emplace_back(*this->board_manager->create_board());
    }

    void recordCommandBuffer(uint32_t imageIndex) {
        vk::CommandBuffer commandBuffer = this->commandBuffers[imageIndex];
        const vk::CommandBufferBeginInfo beginInfo(vk::CommandBufferUsageFlagBits::eOneTimeSubmit,
                                                   nullptr);
        if (commandBuffer.begin(&beginInfo) != vk::Result::eSuccess) {
            throw std::runtime_error("failed to begin recording command buffer");
        }

        std::vector clearValues{vk::ClearValue{vk::ClearColorValue{0.0f, 0.0f, 0.0f, 1.0f}}};
        const vk::RenderPassBeginInfo renderPassInfo(this->renderPass,
                                                     this->swapChainFramebuffers[imageIndex],
                                                     vk::Rect2D({0, 0}, this->swapChainExtent),
                                                     clearValues);
        this->commandBuffers[imageIndex].beginRenderPass(renderPassInfo,
                                                         vk::SubpassContents::eInline);
        const vk::Viewport viewport(0.0f,
                                    0.0f,
                                    static_cast<float>(this->swapChainExtent.width),
                                    static_cast<float>(this->swapChainExtent.height),
                                    0.0f,
                                    1.0f);
        this->commandBuffers[imageIndex].setViewport(0, viewport);
        const vk::Rect2D scissor({0, 0}, swapChainExtent);
        this->commandBuffers[imageIndex].setScissor(0, scissor);

        ImGui_ImplVulkan_RenderDrawData(ImGui::GetDrawData(), this->commandBuffers[imageIndex]);
        this->commandBuffers[imageIndex].endRenderPass();
        this->commandBuffers[imageIndex].end();
    }

    void createSyncObjects() {
        this->queueSubmitFences.reserve(MAX_FRAMES_IN_FLIGHT);
        this->aquireImageSemaphores.reserve(MAX_FRAMES_IN_FLIGHT);
        this->queueSubmitSemaphores.reserve(this->swapChainImages.size());

        constexpr vk::SemaphoreCreateInfo semaphoreInfo{vk::SemaphoreCreateFlags()};
        constexpr vk::FenceCreateInfo fenceInfo{vk::FenceCreateFlagBits::eSignaled};

        for (int i = 0; i < MAX_FRAMES_IN_FLIGHT; ++i) {
            this->queueSubmitFences.push_back(this->ctx->device.createFence(fenceInfo));
            this->aquireImageSemaphores.push_back(this->ctx->device.createSemaphore(semaphoreInfo));
        }
        for (int i = 0; i < this->swapChainImages.size(); ++i) {
            this->queueSubmitSemaphores.push_back(this->ctx->device.createSemaphore(semaphoreInfo));
        }
    }

    void mainLoop() {
        uint32_t inFlightFrame = 0;
        while (!glfwWindowShouldClose(window)) {
            glfwPollEvents();

            if (glfwGetWindowAttrib(window, GLFW_ICONIFIED) != 0) {
                ImGui_ImplGlfw_Sleep(10);
                return;
            }
            ImGui_ImplVulkan_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();
            ImGui::PushStyleVar(ImGuiStyleVar_WindowMinSize, ImVec2(600, 600));
            ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
            ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 0.0f);
            ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 0));
            if (this->board_manager->can_resize()) {
                bool can_resize = true;
                for (const auto& queue_submit_fence : this->queueSubmitFences) {
                    if (this->ctx->device.getFenceStatus(queue_submit_fence)
                        != vk::Result::eSuccess) {
                        can_resize = false;
                        break;
                    }
                }
                if (can_resize) {
                    for (auto& imgui_board : this->imgui_boards) {
                        imgui_board.resize();
                    }
                }
            }
            for (int i = 0; i < this->imgui_boards.size(); ++i) {
                imgui_circuitboard& board = this->imgui_boards[i];
                ImGui::Begin(("Circuitboard " + std::to_string(i)).c_str());
                const ImVec2 board_size = ImGui::GetContentRegionAvail();
                board.get_handle().set_target_size(board_size.x, board_size.y);
                ImGui::ImageButton(("circuitboard" + std::to_string(i)).c_str(),
                                   board.get_imgui_descriptor_set(inFlightFrame),
                                   board_size);
                ImGui::End();
            }
            ImGui::PopStyleVar(4);
            ImGui::ShowDemoWindow();
            ImGui::Render();

            drawFrame(inFlightFrame);
            ++frame;
            inFlightFrame = ++inFlightFrame % MAX_FRAMES_IN_FLIGHT;
        }
    }

    void measure() {
        while (true) {
            std::this_thread::sleep_for(std::chrono::seconds(1));
            std::cout << this->frame << " FPS\n";
            this->frame = 0;
        }
    }

    void drawFrame(const uint32_t inFlightFrame) {
        if (this->ctx->device.waitForFences(this->queueSubmitFences[inFlightFrame],
                                            vk::True,
                                            UINT64_MAX)
            != vk::Result::eSuccess) {
            throw std::runtime_error("failed to wait for the fences");
        }
        const auto nextImage = this->ctx->device
                                   .acquireNextImageKHR(this->swapChain,
                                                        UINT64_MAX,
                                                        this->aquireImageSemaphores[inFlightFrame],
                                                        nullptr);
        if (nextImage.result == vk::Result::eErrorOutOfDateKHR
            || nextImage.result == vk::Result::eSuboptimalKHR || framebufferResized) {
            std::cout << "Recreating swapchain!" << std::endl;
            this->recreateSwapChain();
            this->framebufferResized = false;
            return;
        }
        if (nextImage.result != vk::Result::eSuccess) {
            throw std::runtime_error("failed to acquire next image");
        }
        const uint32_t imageIndex = nextImage.value;

        this->ctx->device.resetFences(this->queueSubmitFences[inFlightFrame]);

        this->commandBuffers[imageIndex].reset();
        this->recordCommandBuffer(imageIndex);

        this->board_manager->render(this->graphicsQueue, inFlightFrame);

        std::vector<vk::PipelineStageFlags>
            waitStages{vk::PipelineStageFlagBits::eColorAttachmentOutput,
                       vk::PipelineStageFlagBits::eFragmentShader};
        std::vector waitSemaphores{this->aquireImageSemaphores[inFlightFrame],
                                   this->board_manager->render_finished_semaphore.get()};
        const vk::SubmitInfo submitInfo{waitSemaphores,
                                        waitStages,
                                        this->commandBuffers[imageIndex],
                                        this->queueSubmitSemaphores[imageIndex]};
        this->graphicsQueue.submit(submitInfo, this->queueSubmitFences[inFlightFrame]);

        vk::PresentInfoKHR presentInfo(this->queueSubmitSemaphores[imageIndex],
                                       this->swapChain,
                                       imageIndex,
                                       nullptr);
        if (this->presentQueue.presentKHR(presentInfo) != vk::Result::eSuccess) {
            throw std::runtime_error("failed to present");
        }
    }

    void cleanup() {
        std::cout << "Cleaning up" << std::endl;
        this->ctx->device.waitIdle();

        this->imgui_boards.clear();
        this->board_manager.reset();

        ImGui_ImplVulkan_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
        this->ctx->device.destroyDescriptorPool(this->imguiDescriptorPool);

        this->cleanupSwapChain();

        for (auto queue_submit_fence : this->queueSubmitFences) {
            this->ctx->device.destroyFence(queue_submit_fence);
        }
        for (auto busy_aquire_semaphore : this->aquireImageSemaphores) {
            this->ctx->device.destroySemaphore(busy_aquire_semaphore);
        }
        for (auto queue_submit_semaphore : this->queueSubmitSemaphores) {
            this->ctx->device.destroySemaphore(queue_submit_semaphore);
        }

        this->ctx->device.destroyCommandPool(this->commandPool);
        this->ctx->device.destroyPipeline(pipeline);
        this->ctx->device.destroyPipelineLayout(this->pipelineLayout);
        this->ctx->device.destroyRenderPass(this->renderPass);
        this->ctx->device.destroy();

        if (enableValidationLayers) {
            pfnVkDestroyDebugUtilsMessengerEXT(this->instance, this->debugMessenger, nullptr);
        }

        this->instance.destroySurfaceKHR(this->surface);
        this->instance.destroy();

        glfwDestroyWindow(window);
        glfwTerminate();
    }

    GLFWwindow* window;
    vk::Instance instance;
    vk::DebugUtilsMessengerEXT debugMessenger;
    vk::Queue graphicsQueue;
    vk::Queue presentQueue;
    vk::SurfaceKHR surface;
    vk::SwapchainKHR swapChain;
    std::vector<vk::Image> swapChainImages;
    vk::Format swapChainImageFormat;
    vk::Extent2D swapChainExtent;
    std::vector<vk::ImageView> swapChainImageViews;
    vk::RenderPass renderPass;
    vk::PipelineLayout pipelineLayout;
    vk::Pipeline pipeline;
    std::vector<vk::Framebuffer> swapChainFramebuffers;
    vk::CommandPool commandPool;
    std::vector<vk::CommandBuffer> commandBuffers;

    std::vector<vk::Semaphore> aquireImageSemaphores;
    std::vector<vk::Semaphore> queueSubmitSemaphores;
    std::vector<vk::Fence> queueSubmitFences;

    std::atomic<int> frame = 0;

    bool framebufferResized = false;

    vk::DescriptorPool imguiDescriptorPool;

    std::unique_ptr<vulkan_context> ctx = std::make_unique<vulkan_context>();
    std::unique_ptr<circuitboard_manager> board_manager;
    std::vector<imgui_circuitboard> imgui_boards;
};

int main() {
    try {
        HelloTriangleApplication app;
        app.run();
        std::cout << "Closing window" << std::endl;
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}