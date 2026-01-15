//
// Created by felix on 8/3/25.
//

#ifndef SHADER_HPP
#define SHADER_HPP
#include <filesystem>
#include <fstream>
#include <vector>
#include <vulkan/vulkan.hpp>

#include "environment.hpp"

namespace bit {

static std::vector<uint32_t> read_shader(const std::string &filename) {
    std::filesystem::path path = SHADER_DIR;
    if (char *val = std::getenv("SHADER_DIR"); val) {
        path = std::string(val);
    }
    path = path / filename;
    std::ifstream file(path, std::ios::ate | std::ios::binary);
    if (!file.is_open()) {
        throw std::runtime_error("failed to load shader file at " + path.string());
    }

    size_t file_size = file.tellg();
    if (file_size % sizeof(uint32_t) != 0) {
        throw std::runtime_error("Shader file size is not a multiple of 4 bytes (uint32_t).");
    }

    std::vector<uint32_t> buffer(file_size / sizeof(uint32_t));

    file.seekg(0);
    file.read(reinterpret_cast<char *>(buffer.data()), file_size);
    file.close();

    return buffer;
}

static vk::ShaderModule create_shader_module(const vk::Device &device,
                                             const std::vector<uint32_t> &code) {
    const vk::ShaderModuleCreateInfo create_info = {vk::ShaderModuleCreateFlags{}, code, nullptr};
    return device.createShaderModule(create_info);
}

static vk::UniqueShaderModule create_shader_module_unique(const vk::Device &device,
                                                          const std::vector<uint32_t> &code) {
    const vk::ShaderModuleCreateInfo create_info = {vk::ShaderModuleCreateFlags{}, code, nullptr};
    return device.createShaderModuleUnique(create_info);
}

static vk::ShaderModule load_shader(const vk::Device &device, const std::string &filename) {
    return create_shader_module(device, read_shader(filename));
}

static vk::UniqueShaderModule load_shader_unique(const vk::Device &device,
                                                 const std::string &filename) {
    return create_shader_module_unique(device, read_shader(filename));
}
}

#endif //SHADER_HPP
