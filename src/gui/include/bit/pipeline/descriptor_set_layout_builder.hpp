//
// Created by felix on 1/4/26.
//

#ifndef BUILDIT_TEST_DESCRIPTOR_SET_LAYOUT_BUILDER_HPP
#define BUILDIT_TEST_DESCRIPTOR_SET_LAYOUT_BUILDER_HPP
#include <vulkan/vulkan.hpp>


class descriptor_set_layout_builder_t {
public:
    explicit descriptor_set_layout_builder_t(const vk::Device &device);

    descriptor_set_layout_builder_t &add_binding(uint index,
                                                 vk::DescriptorType descriptor_type,
                                                 uint descriptor_count,
                                                 vk::ShaderStageFlags stages);


    [[nodiscard]] vk::UniqueDescriptorSetLayout build() const;

private:
    std::vector<vk::DescriptorSetLayoutBinding> bindings;
    const vk::Device &device;

};


#endif //BUILDIT_TEST_DESCRIPTOR_SET_LAYOUT_BUILDER_HPP