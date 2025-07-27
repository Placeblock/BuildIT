//
// Created by felix on 19.07.25.
//

#ifndef IMGUI_CIRCUITBOARD_HPP
#define IMGUI_CIRCUITBOARD_HPP
#include "circuitboard.hpp"

class imgui_circuitboard {
public:
    explicit imgui_circuitboard(circuit_board& board);

    [[nodiscard]] circuit_board& get_handle() const;
    const VkDescriptorSet& get_imgui_descriptor_set(uint32_t index) const;

    void resize(uint32_t image_index);

    bool update_in_flight_frames(uint32_t in_flight_frames);

private:
    circuit_board& board;

    std::vector<VkDescriptorSet> imgui_descriptor_sets;
};

#endif //IMGUI_CIRCUITBOARD_HPP
