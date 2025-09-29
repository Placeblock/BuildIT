//
// Created by felix on 8/3/25.
//

#ifndef SHADER_HPP
#define SHADER_HPP
#include "vulkancontext.hpp"
#include <filesystem>
#include <fstream>

const std::filesystem::path SHADER_DIR = {"shaders/"};

static std::vector<uint32_t> readShader(const std::string &filename) {
    std::filesystem::path path = SHADER_DIR / filename;
    std::ifstream file(path, std::ios::ate | std::ios::binary);
    if (!file.is_open()) {
        throw std::runtime_error("failed to open file!");
    }

    size_t fileSize = file.tellg();
    if (fileSize % sizeof(uint32_t) != 0) {
        throw std::runtime_error("Shader file size is not a multiple of 4 bytes (uint32_t).");
    }

    std::vector<uint32_t> buffer(fileSize / sizeof(uint32_t));

    file.seekg(0);
    file.read(reinterpret_cast<char *>(buffer.data()), fileSize);
    file.close();

    return buffer;
}

static vk::ShaderModule createShaderModule(const vulkan_context &ctx,
                                           const std::vector<uint32_t> &code) {
    const vk::ShaderModuleCreateInfo createInfo = {vk::ShaderModuleCreateFlags{}, code, nullptr};
    return ctx.device.createShaderModule(createInfo);
}
static vk::UniqueShaderModule createShaderModuleUnique(const vk::Device &device,
                                                       const std::vector<uint32_t> &code) {
    const vk::ShaderModuleCreateInfo createInfo = {vk::ShaderModuleCreateFlags{}, code, nullptr};
    return device.createShaderModuleUnique(createInfo);
}

static vk::ShaderModule loadShader(const vulkan_context &ctx, const std::string &filename) {
    return createShaderModule(ctx, readShader(filename));
}
static vk::UniqueShaderModule loadShaderUnique(const vk::Device &device,
                                               const std::string &filename) {
    return createShaderModuleUnique(device, readShader(filename));
}

#endif //SHADER_HPP
