//
// Created by felix on 19.07.25.
//

#include "app/vulkan/imgui_circuitboard.hpp"

#include "imgui_impl_vulkan.h"

circuit_board &imgui_circuitboard::get_handle() const {
    return this->board;
}

imgui_circuitboard::imgui_circuitboard(circuit_board &board) : board(board) {
    for (const auto &image : this->board.images) {
        this->imgui_descriptor_sets.push_back(
            ImGui_ImplVulkan_AddTexture(this->board.sampler,
                                        // vk::Sampler
                                        *image.view,
                                        // vk::ImageView
                                        VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL));
    }
}

void imgui_circuitboard::resize(const uint32_t image_index) {
    ImGui_ImplVulkan_RemoveTexture(this->imgui_descriptor_sets[image_index]);
    this->board.resize(image_index);
    this->imgui_descriptor_sets[image_index]
        = ImGui_ImplVulkan_AddTexture(this->board.sampler,
                                      // vk::Sampler
                                      *this->board.images[image_index].view,
                                      // vk::ImageView
                                      VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
}

const VkDescriptorSet &imgui_circuitboard::get_imgui_descriptor_set(const uint32_t index) const {
    return this->imgui_descriptor_sets.at(index);
}