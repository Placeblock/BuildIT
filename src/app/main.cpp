#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
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

    void createLogicalDevice() {
        const std::vector<vk::QueueFamilyProperties> properties = this->physicalDevice
                                                                      .getQueueFamilyProperties();
        const queue_family_indices family_indices = this->find_queue_families(properties);
        std::vector<vk::DeviceQueueCreateInfo> queue_create_infos;
        constexpr float queuePriority = 1.0f;
        for (uint32_t queue_family : family_indices) {
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
        this->graphicsQueue = this->device.getQueue(family_indices.graphics_family, 0);
        this->presentQueue = this->device.getQueue(family_indices.present_family, 0);
    }

    void createSurface() {
        VkSurfaceKHR _surface;
        if (glfwCreateWindowSurface(this->instance, this->window, nullptr, &_surface)
            != VK_SUCCESS) {
            throw std::runtime_error("failed to create window surface");
        }
        this->surface = vk::SurfaceKHR(_surface);
    }

    void mainLoop() {
        /*while (!glfwWindowShouldClose(window)) {
            glfwPollEvents();
        }*/
    }

    void cleanup() {
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