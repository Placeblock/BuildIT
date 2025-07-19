//
// Created by felix on 19.07.25.
//

#include "app/vulkan/imgui_circuitboard.hpp"

#include "imgui_impl_vulkan.h"

circuit_board& imgui_circuitboard::get_handle() const {
    return this->board;
}

imgui_circuitboard::imgui_circuitboard(circuit_board& board) : board(board) {
    this->create_imgui_descriptor_sets();
}

bool imgui_circuitboard::resize(const int width, const int height) {
    const bool resized = this->board.resize(width, height);
    if (resized) {
        this->create_imgui_descriptor_sets();
    }
    return resized;
}

const VkDescriptorSet& imgui_circuitboard::get_imgui_descriptor_set(const uint32_t index) const {
    return this->imgui_descriptor_sets.at(index);
}

void imgui_circuitboard::create_imgui_descriptor_sets() {
    for (const auto existing_set : this->imgui_descriptor_sets) {
        ImGui_ImplVulkan_RemoveTexture(existing_set);
    }
    this->imgui_descriptor_sets.clear();
    for (auto& image_view : this->board.image_views) {
        this->imgui_descriptor_sets.push_back(
            ImGui_ImplVulkan_AddTexture(this->board.sampler, // vk::Sampler
                                        *image_view,         // vk::ImageView
                                        VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL));
    }
}