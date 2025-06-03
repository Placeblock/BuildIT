#include <GLFW/glfw3.h>
#include <any>
#include <iostream>
#include <spdlog/spdlog.h>
#include <vulkan/vulkan.hpp>

constexpr uint32_t WIDTH = 800;
constexpr uint32_t HEIGHT = 600;

const std::vector VALIDATION_LAYERS = {"VK_LAYER_KHRONOS_validation"};

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
        createInfo.enabledLayerCount = 0;
        if (enableValidationLayers) {
            createInfo.enabledLayerCount = static_cast<uint32_t>(VALIDATION_LAYERS.size());
            createInfo.ppEnabledLayerNames = VALIDATION_LAYERS.data();
        } else {
            createInfo.enabledLayerCount = 0;
        }
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
        if (!enableValidationLayers)
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
        pfnVkDestroyDebugUtilsMessengerEXT = reinterpret_cast<PFN_vkDestroyDebugUtilsMessengerEXT>(
            instance.getProcAddr("vkDestroyDebugUtilsMessengerEXT"));

        this->debugMessenger = instance.createDebugUtilsMessengerEXT(createInfo);
    }

    void mainLoop() {
        while (!glfwWindowShouldClose(window)) {
            glfwPollEvents();
        }
    }

    void cleanup() {
        this->instance.destroy();

        glfwDestroyWindow(window);

        glfwTerminate();
    }

    GLFWwindow* window;
    vk::Instance instance;
    vk::DebugUtilsMessengerEXT debugMessenger;
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