//
// Created by felix on 1/9/26.
//

#ifndef BUILDIT_TEST_DEBUG_NAMES_HPP
#define BUILDIT_TEST_DEBUG_NAMES_HPP
#include <vulkan/vulkan.hpp>

namespace bit {
inline PFN_vkSetDebugUtilsObjectNameEXT pfn_vk_set_debug_utils_name_ext;

template<typename T>
void set_name(vk::Device &device, T &obj, const std::string &name) {
#ifndef NDEBUG
    const typename T::CType handle = obj;
    const VkDebugUtilsObjectNameInfoEXT name_info = {
        .sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_OBJECT_NAME_INFO_EXT,
        .pNext = nullptr,
        .objectType = static_cast<VkObjectType>(T::objectType),
        .objectHandle = reinterpret_cast<std::uint64_t>(handle),
        .pObjectName = name.c_str()
    };
    pfn_vk_set_debug_utils_name_ext(device, &name_info);
#endif
}

inline void set_name(const vk::Device &device, VkDeviceMemory obj, const std::string &name) {
#ifndef NDEBUG
    const VkDebugUtilsObjectNameInfoEXT name_info = {
        .sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_OBJECT_NAME_INFO_EXT,
        .pNext = nullptr,
        .objectType = VK_OBJECT_TYPE_DEVICE_MEMORY,
        .objectHandle = reinterpret_cast<std::uint64_t>(obj),
        .pObjectName = name.c_str()
    };
    pfn_vk_set_debug_utils_name_ext(device, &name_info);
#endif
}

inline void init_debug_names(const vk::Instance &instance) {
    pfn_vk_set_debug_utils_name_ext = reinterpret_cast<
        PFN_vkSetDebugUtilsObjectNameEXT>(
        instance.getProcAddr("vkSetDebugUtilsObjectNameEXT"));
    if (pfn_vk_set_debug_utils_name_ext == nullptr) {
        throw std::runtime_error("vkSetDebugUtilsObjectNameEXT not found");
    }
}
}

#endif //BUILDIT_TEST_DEBUG_NAMES_HPP