//
// Created by felix on 06.06.25.
//

#include "glslang/Public/ResourceLimits.h"
#include <glslang/Public/ShaderLang.h>
#include <glslang/SPIRV/GlslangToSpv.h>
#include <vulkan/vulkan.hpp>

EShLanguage translateShaderStage(const vk::ShaderStageFlagBits stage) {
    switch (stage) {
    case vk::ShaderStageFlagBits::eVertex:
        return EShLangVertex;
    case vk::ShaderStageFlagBits::eTessellationControl:
        return EShLangTessControl;
    case vk::ShaderStageFlagBits::eTessellationEvaluation:
        return EShLangTessEvaluation;
    case vk::ShaderStageFlagBits::eGeometry:
        return EShLangGeometry;
    case vk::ShaderStageFlagBits::eFragment:
        return EShLangFragment;
    case vk::ShaderStageFlagBits::eCompute:
        return EShLangCompute;
    case vk::ShaderStageFlagBits::eRaygenNV:
        return EShLangRayGenNV;
    case vk::ShaderStageFlagBits::eAnyHitNV:
        return EShLangAnyHitNV;
    case vk::ShaderStageFlagBits::eClosestHitNV:
        return EShLangClosestHitNV;
    case vk::ShaderStageFlagBits::eMissNV:
        return EShLangMissNV;
    case vk::ShaderStageFlagBits::eIntersectionNV:
        return EShLangIntersectNV;
    case vk::ShaderStageFlagBits::eCallableNV:
        return EShLangCallableNV;
    case vk::ShaderStageFlagBits::eTaskNV:
        return EShLangTaskNV;
    case vk::ShaderStageFlagBits::eMeshNV:
        return EShLangMeshNV;
    default:
        assert(false && "Unknown shader stage");
        return EShLangVertex;
    }
}

void GLSLtoSPV(const vk::ShaderStageFlagBits shaderType,
               std::string const& glslShader,
               std::vector<unsigned int>& spvShader) {
    const EShLanguage stage = translateShaderStage(shaderType);
    const char* shaderStrings[1];
    shaderStrings[0] = glslShader.data();
    glslang::TShader shader(stage);
    shader.setStrings(shaderStrings, 1);
    EShMessages messages = (EShMessages) (EShMsgSpvRules | EShMsgVulkanRules);
    if (!shader.parse(GetDefaultResources(), 100, false, messages)) {
        puts(shader.getInfoLog());
        puts(shader.getInfoDebugLog());
        throw std::runtime_error("failed to parse shader code");
    }
    glslang::TProgram program;
    program.addShader(&shader);

    //
    // Program-level processing...
    //

    if (!program.link(messages)) {
        puts(shader.getInfoLog());
        puts(shader.getInfoDebugLog());
        fflush(stdout);
        throw std::runtime_error("failed to link shader code");
    }

    glslang::GlslangToSpv(*program.getIntermediate(stage), spvShader);
}

vk::ShaderModule createShaderModule(const vk::Device& device,
                                    const vk::ShaderStageFlagBits shaderStage,
                                    const std::string& shaderText) {
    std::vector<unsigned int> shaderSPV;
    GLSLtoSPV(shaderStage, shaderText, shaderSPV);
    return device.createShaderModule(
        vk::ShaderModuleCreateInfo(vk::ShaderModuleCreateFlags(), shaderSPV));
}