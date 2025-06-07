#define GLFW_INCLUDE_VULKAN
#include "shader.cpp"
#include <GLFW/glfw3.h>
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

struct queue_family_indices {
    uint32_t graphics_family;
    uint32_t present_family;
};
struct swap_chain_support_details {
    vk::SurfaceCapabilitiesKHR capabilities;
    std::vector<vk::SurfaceFormatKHR> formats;
    std::vector<vk::PresentModeKHR> present_modes;
};

static std::string readFile(const std::string& filename) {
    std::ifstream file(filename, std::ios::ate | std::ios::binary);
    if (!file.is_open()) {
        throw std::runtime_error("failed to open file!");
    }
    std::stringstream buffer;
    buffer << file.rdbuf();

    return buffer.str();
}

class HelloTriangleApplication {
public:
    void run() {
        initWindow();
        initVulkan();
        mainLoop();
        cleanup();
    }

private:
    void initWindow() {
        glfwInit();

        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

        window = glfwCreateWindow(WIDTH, HEIGHT, "Vulkan", nullptr, nullptr);
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
        this->createGraphicsPipeline();
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
                  void* userData) {
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
        vk::DebugUtilsMessengerCreateInfoEXT
            createInfo({},
                       vk::DebugUtilsMessageSeverityFlagBitsEXT::eError
                           | vk::DebugUtilsMessageSeverityFlagBitsEXT::eWarning,
                       vk::DebugUtilsMessageTypeFlagBitsEXT::eGeneral
                           | vk::DebugUtilsMessageTypeFlagBitsEXT::ePerformance
                           | vk::DebugUtilsMessageTypeFlagBitsEXT::eValidation,
                       debugCallback);

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

        this->debugMessenger = instance.createDebugUtilsMessengerEXT(createInfo);
    }

    void pickPhsicalDevice() {
        const std::vector<vk::PhysicalDevice> devices = instance.enumeratePhysicalDevices();
        std::multimap<int, vk::PhysicalDevice> candidates;
        for (auto physical_device : devices) {
            int score = this->weightDevice(physical_device);
            candidates.insert(std::make_pair(score, physical_device));
        }
        this->physicalDevice = candidates.begin()->second;
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
        if (this->physicalDevice.getSurfaceSupportKHR(static_cast<uint32_t>(graphics_family_index),
                                                      this->surface)) {
            indices.present_family = graphics_family_index;
        } else {
            size_t present_family_index = -1;
            for (int i = 0; i < queue_families.size(); ++i) {
                if (this->physicalDevice.getSurfaceSupportKHR(static_cast<uint32_t>(i),
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
        return vk::PresentModeKHR::eFifo;
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
        const std::vector<vk::QueueFamilyProperties> properties = this->physicalDevice
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
        this->device = this->physicalDevice.createDevice(deviceCreateInfo);
        this->graphicsQueue = this->device.getQueue(graphics_family, 0);
        this->presentQueue = this->device.getQueue(present_family, 0);
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
            this->physicalDevice);
        vk::SurfaceFormatKHR surface_format = this->choose_swap_surface_format(
            swap_chain_support.formats);
        vk::PresentModeKHR present_mode = this->choose_swap_present_mode(
            swap_chain_support.present_modes);
        vk::Extent2D extent = this->choose_swap_extent(swap_chain_support.capabilities);

        uint32_t imageCount = swap_chain_support.capabilities.minImageCount + 1;
        if (swap_chain_support.capabilities.maxImageCount > 0
            && imageCount > swap_chain_support.capabilities.maxImageCount) {
            imageCount = swap_chain_support.capabilities.maxImageCount;
        }
        vk::SharingMode sharingMode;
        const std::vector<vk::QueueFamilyProperties> properties = physicalDevice
                                                                      .getQueueFamilyProperties();
        queue_family_indices indices = find_queue_families(properties);
        std::vector<uint32_t> queue_family_indices;
        if (indices.graphics_family != indices.present_family) {
            sharingMode = vk::SharingMode::eConcurrent;
            queue_family_indices = {indices.graphics_family, indices.present_family};
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
                                                          vk::True,
                                                          nullptr);
        this->swapChain = this->device.createSwapchainKHR(swap_chain_create_info);
        this->swapChainImages = this->device.getSwapchainImagesKHR(this->swapChain);
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
            this->swapChainImageViews.push_back(device.createImageView(imageViewCreateInfo));
        }
    }

    void createGraphicsPipeline() {
        const std::string vertShader = readFile("shader.vert");
        const std::string fragShader = readFile("shader.frag");

        const vk::ShaderModule vertModule = createShaderModule(this->device,
                                                               vk::ShaderStageFlagBits::eVertex,
                                                               vertShader);
        const vk::ShaderModule fragModule = createShaderModule(this->device,
                                                               vk::ShaderStageFlagBits::eFragment,
                                                               fragShader);

        vk::PipelineShaderStageCreateInfo vertShaderStageInfo(vk::PipelineShaderStageCreateFlags(),
                                                              vk::ShaderStageFlagBits::eVertex,
                                                              vertModule,
                                                              "main");
        vk::PipelineShaderStageCreateInfo fragShaderStageInfo(vk::PipelineShaderStageCreateFlags(),
                                                              vk::ShaderStageFlagBits::eFragment,
                                                              fragModule,
                                                              "main");
        vk::PipelineShaderStageCreateInfo shaderStages[] = {vertShaderStageInfo,
                                                            fragShaderStageInfo};
        vk::PipelineVertexInputStateCreateInfo
            vertexInputInfo(vk::PipelineVertexInputStateCreateFlags(), 0, nullptr, 0, nullptr);
        vk::PipelineInputAssemblyStateCreateInfo
            inputAssembly(vk::PipelineInputAssemblyStateCreateFlags(),
                          vk::PrimitiveTopology::eTriangleList,
                          vk::True);
        vk::Viewport viewport(0,
                              0,
                              this->swapChainExtent.width,
                              this->swapChainExtent.height,
                              0.0f,
                              1.0f);
        vk::Rect2D scissor({0, 0}, this->swapChainExtent);

        std::vector<vk::DynamicState> dynamicStates = {vk::DynamicState::eViewport,
                                                       vk::DynamicState::eScissor};

        vk::PipelineDynamicStateCreateInfo dynamicState(vk::PipelineDynamicStateCreateFlags(),
                                                        dynamicStates);

        vk::PipelineViewportStateCreateInfo viewportState(vk::PipelineViewportStateCreateFlags(),
                                                          1,
                                                          nullptr,
                                                          1,
                                                          nullptr);

        vk::PipelineRasterizationStateCreateInfo
            rasterizer(vk::PipelineRasterizationStateCreateFlags(),
                       vk::False,
                       vk::False,
                       vk::PolygonMode::eFill,
                       vk::CullModeFlagBits::eBack,
                       vk::FrontFace::eClockwise,
                       vk::False,
                       0.0f,
                       0.0f,
                       0.0f,
                       1.0f);
        vk::PipelineMultisampleStateCreateInfo
            multisampling(vk::PipelineMultisampleStateCreateFlags(),
                          vk::SampleCountFlagBits::e1,
                          vk::False,
                          1.0f,
                          nullptr,
                          vk::False,
                          vk::False);

        vk::PipelineColorBlendAttachmentState
            colorBlendAttachment(vk::False,
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
                                                            vk::False,
                                                            vk::LogicOp::eCopy,
                                                            colorBlendAttachment,
                                                            {0.0, 0.0, 0.0, 0.0});
        vk::PipelineLayoutCreateInfo layoutInfo(vk::PipelineLayoutCreateFlags(), nullptr, nullptr);
        this->pipelineLayout = device.createPipelineLayout(layoutInfo);

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
                                                    pipelineLayout,
                                                    renderPass,
                                                    0,
                                                    {},
                                                    -1);
        auto graphics_pipeline = device.createGraphicsPipeline(nullptr, pipelineInfo);
        if (graphics_pipeline.result != vk::Result::eSuccess) {
            throw std::runtime_error("failed to create graphics pipeline");
        }
        this->pipeline = graphics_pipeline.value;

        this->device.destroyShaderModule(vertModule);
        this->device.destroyShaderModule(fragModule);
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
                                       colorAttachmentRef);
        const vk::RenderPassCreateInfo renderPassInfo(vk::RenderPassCreateFlags(),
                                                      colorAttachment,
                                                      subpass);
        this->renderPass = device.createRenderPass(renderPassInfo);
    }

    void mainLoop() {
        while (!glfwWindowShouldClose(window)) {
            glfwPollEvents();
        }
    }

    void cleanup() {
        this->device.destroyPipeline(pipeline);
        this->device.destroyPipelineLayout(this->pipelineLayout);
        this->device.destroyRenderPass(this->renderPass);
        for (const auto& imageView : this->swapChainImageViews) {
            device.destroyImageView(imageView);
        }
        this->device.destroySwapchainKHR(this->swapChain);
        this->device.destroy();

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
    vk::PhysicalDevice physicalDevice;
    vk::Device device;
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
};

int main() {
    try {
        HelloTriangleApplication app;
        app.run();
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}