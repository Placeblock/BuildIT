//
// Created by felix on 1/5/26.
//

#ifndef BUILDIT_TEST_CHIP_BUFFER_HPP
#define BUILDIT_TEST_CHIP_BUFFER_HPP
#include "bit/queue_family.hpp"
#include <entt/entt.hpp>
#include "bit/memory/memory_allocator.hpp"
#include "bit/debug-names.hpp"
#include "bit/shader.hpp"
#include "bit/pipeline/build_pipeline.hpp"
#include "glm/vec2.hpp"
#include "glm/vec4.hpp"
#include "glm/mat3x3.hpp"

#include "bounding_box.hpp"

#ifndef FRAMES_IN_FLIGHT
#define FRAMES_IN_FLIGHT (size_t) 3
#endif

template<typename T>
class chip_buffer {
    using data_traits_type = entt::component_traits<T>;

public:
    chip_buffer(vk::Device &device,
                bit::queue_family_indices_t &queue_family_indices,
                bit::memory_allocator &mem_allocator,
                bit::pipeline &culling_pipeline,
                bit::pipeline &reset_culling_pipeline,
                bit::pipeline &graphics_pipeline,
                const vk::DescriptorSetLayout &culling_layout,
                const vk::DescriptorSetLayout &graphics_layout,
                vk::DescriptorPool &compute_descriptor_pool,
                vk::DescriptorPool &graphics_descriptor_pool,
                entt::registry &registry,
                vk::Extent2D &extent) : device(device),
                                        queue_family_indices(
                                            queue_family_indices),
                                        mem_allocator(mem_allocator),
                                        position_storage(registry.storage<bounding_box_t>()),
                                        simulation_data_storage(registry.storage<T>()),
                                        culling_pipeline(culling_pipeline),
                                        reset_culling_pipeline(reset_culling_pipeline),
                                        graphics_pipeline(graphics_pipeline),
                                        extent(extent) {
        for (int i = 0; i < 100; ++i) {
            const auto entity = registry.create();
            registry.emplace<bounding_box_t>(entity, glm::vec4{i * 5, i * 5, 20, 20});
            registry.emplace<T>(entity);
        }
        this->buffer_capacities = std::vector<size_t>(
            FRAMES_IN_FLIGHT,
            -1);
        std::vector set_layouts(FRAMES_IN_FLIGHT, culling_layout);
        this->compute_descriptor_sets = this->device.allocateDescriptorSetsUnique({
            compute_descriptor_pool, set_layouts});
        std::vector graphics_set_layouts(FRAMES_IN_FLIGHT, graphics_layout);
        this->graphics_descriptor_sets = this->device.allocateDescriptorSetsUnique({
            graphics_descriptor_pool, graphics_set_layouts});
        for (size_t i = 0; i < FRAMES_IN_FLIGHT; i++) {
            compute_indirect_command_buffers.emplace_back();
            position_staging_buffers.emplace_back();
            position_culling_buffers.emplace_back();
            culled_indices_buffers.emplace_back();

            simulation_data_staging_buffers.emplace_back();
            simulation_data_buffers.emplace_back();

            this->compute_indirect_command_buffers[i] = this->mem_allocator.
                template allocate_buffer<uint32_t>(4,
                                                   vk::BufferUsageFlagBits::eStorageBuffer |
                                                   vk::BufferUsageFlagBits::eIndirectBuffer,
                                                   this->queue_family_indices.
                                                   compute_family,
                                                   false);
            bit::set_name(this->device,
                          this->compute_indirect_command_buffers[i]->buffer,
                          "compute-indirect-command-buffer-" + std::to_string(i));
            bit::set_name(this->device,
                          this->compute_indirect_command_buffers[i]->allocation->
                          GetMemory(),
                          "compute-indirect-command-buffer-memory" + std::to_string(i));
        }
    }

    void pre_record_buffer(const vk::CommandBuffer &transfer_buffer,
                           const vk::CommandBuffer &compute_buffer,
                           const vk::CommandBuffer &graphics_buffer,
                           const uint in_flight_frame) {
        // TODO: AFTER FLICKER BUCK IS FIXED; DO WE STILL NED THIS?
        if (this->simulation_data_storage.capacity() == 0)
            return;
        if (this->buffer_capacities[in_flight_frame] != this->simulation_data_storage.capacity()) {
            spdlog::info("creating new buffers for chip because of capacity mismatch {}/{}",
                         this->buffer_capacities[in_flight_frame],
                         this->simulation_data_storage.capacity());
            this->buffer_capacities[in_flight_frame] = this->simulation_data_storage.capacity();
            this->allocate_buffers(in_flight_frame);
            this->write_descriptor_set(in_flight_frame);
            spdlog::info("finished creating new buffers");
        }
        spdlog::info("{}|{}",
                     this->position_storage.size(),
                     this->buffer_capacities[in_flight_frame]);

        const vk::BufferMemoryBarrier culled_indices_memory_barrier = {
            vk::AccessFlagBits::eShaderWrite,
            vk::AccessFlagBits::eShaderRead, vk::QueueFamilyIgnored,
            vk::QueueFamilyIgnored, this->culled_indices_buffers[in_flight_frame]->buffer,
            0, vk::WholeSize
        };
        graphics_buffer.pipelineBarrier(
            vk::PipelineStageFlagBits::eComputeShader,
            vk::PipelineStageFlagBits::eVertexShader,
            {},
            {},
            culled_indices_memory_barrier,
            nullptr);
    }

    bool record_buffer(const vk::CommandBuffer &transfer_buffer,
                       const vk::CommandBuffer &compute_buffer,
                       const vk::CommandBuffer &graphics_buffer,
                       const uint in_flight_frame) {
        if (this->simulation_data_storage.capacity() == 0)
            return false;

        for (int i = 0; i < this->position_storage.capacity() / entt::component_traits<
                            bounding_box_t>::page_size; ++i) {
            void *mapped_data = this->position_staging_buffers[in_flight_frame]->info.pMappedData;
            mapped_data = static_cast<bounding_box_t *>(mapped_data) + i * entt::component_traits<
                              bounding_box_t>::page_size;
            std::memcpy(mapped_data,
                        this->position_storage.raw()[i],
                        sizeof(bounding_box_t) * entt::component_traits<bounding_box_t>::page_size);
        }
        for (int i = 0; i < this->simulation_data_storage.capacity() / data_traits_type::page_size;
             ++i) {
            void *mapped_data = this->simulation_data_staging_buffers[in_flight_frame]->info.
                pMappedData;
            mapped_data = static_cast<T *>(mapped_data) + i * data_traits_type::page_size;
            std::memcpy(mapped_data,
                        this->simulation_data_storage.raw()[i],
                        sizeof(T) * data_traits_type::page_size);
        }

        // Transfer

        const vk::BufferMemoryBarrier transfer_pos_memory_accept_back_barrier{
            {}, vk::AccessFlagBits::eTransferWrite,
            vk::QueueFamilyIgnored,
            vk::QueueFamilyIgnored,
            this->position_culling_buffers[in_flight_frame]->buffer, 0,
            vk::WholeSize};
        transfer_buffer.pipelineBarrier(
            vk::PipelineStageFlagBits::eTopOfPipe,
            vk::PipelineStageFlagBits::eTransfer,
            {},
            {},
            transfer_pos_memory_accept_back_barrier,
            {});
        transfer_buffer.copyBuffer(
            this->position_staging_buffers[in_flight_frame]->buffer,
            this->position_culling_buffers[in_flight_frame]->buffer,
            {vk::BufferCopy{0, 0, sizeof(bounding_box_t) * this->position_storage.size()}});
        transfer_buffer.copyBuffer(
            this->simulation_data_staging_buffers[in_flight_frame]->buffer,
            this->simulation_data_buffers[in_flight_frame]->buffer,
            {vk::BufferCopy{0, 0, sizeof(T) * this->simulation_data_storage.size()}});

        // Compute

        const vk::BufferMemoryBarrier compute_culling_memory_barrier{
            {}, vk::AccessFlagBits::eShaderRead,
            vk::QueueFamilyIgnored,
            vk::QueueFamilyIgnored,
            this->position_culling_buffers[in_flight_frame]->buffer, 0,
            vk::WholeSize};
        compute_buffer.pipelineBarrier(
            vk::PipelineStageFlagBits::eTopOfPipe,
            vk::PipelineStageFlagBits::eComputeShader,
            {},
            {},
            compute_culling_memory_barrier,
            {});
        compute_buffer.bindDescriptorSets(vk::PipelineBindPoint::eCompute,
                                          *this->reset_culling_pipeline.layout,
                                          0,
                                          *this->compute_descriptor_sets[in_flight_frame],
                                          {});
        compute_buffer.bindPipeline(vk::PipelineBindPoint::eCompute,
                                    *this->reset_culling_pipeline.handle);
        compute_buffer.dispatch(1, 1, 1);
        constexpr vk::MemoryBarrier memory_barrier = {
            vk::AccessFlagBits::eShaderWrite,
            vk::AccessFlagBits::eShaderRead | vk::AccessFlagBits::eShaderWrite
        };
        compute_buffer.pipelineBarrier(
            vk::PipelineStageFlagBits::eComputeShader,
            vk::PipelineStageFlagBits::eComputeShader,
            {},
            memory_barrier,
            nullptr,
            nullptr);
        compute_buffer.bindPipeline(vk::PipelineBindPoint::eCompute,
                                    *this->culling_pipeline.handle);
        const glm::vec4 constants{0, 0, this->extent.width, this->extent.height};
        compute_buffer.pushConstants(*this->culling_pipeline.layout,
                                     vk::ShaderStageFlagBits::eCompute,
                                     0,
                                     4 * sizeof(float),
                                     &constants);
        compute_buffer.dispatch(this->position_storage.size(), 1, 1);

        // Graphics
        graphics_buffer.bindPipeline(vk::PipelineBindPoint::eGraphics,
                                     *this->graphics_pipeline.handle);
        const std::vector<vk::DescriptorSet> descriptor_sets = {
            *this->compute_descriptor_sets[in_flight_frame],
            *this->graphics_descriptor_sets[in_flight_frame]
        };
        graphics_buffer.bindDescriptorSets(vk::PipelineBindPoint::eGraphics,
                                           *this->graphics_pipeline.layout,
                                           0,
                                           descriptor_sets,
                                           {});
        const glm::mat4 projection_matrix{2.0 / this->extent.width, 0, 0, 0, 0,
                                          2.0 / this->extent.height,
                                          0, 0, -1,
                                          -1, 1, 0, 0, 0, 0, 0};
        graphics_buffer.pushConstants(*this->graphics_pipeline.layout,
                                      vk::ShaderStageFlagBits::eVertex,
                                      0,
                                      sizeof(glm::mat4),
                                      &projection_matrix);
        graphics_buffer.drawIndirect(
            vk::Buffer(*this->compute_indirect_command_buffers[in_flight_frame]),
            0,
            1,
            0);

        return true;
    }

private:
    void allocate_buffers() {
        for (int i = 0; i < FRAMES_IN_FLIGHT; ++i) {
            this->allocate_buffers(i);
        }
    }

    void allocate_buffers(uint32_t in_flight_frame) {
        std::vector<uint32_t> culling_buffer_queue_families = {
            this->queue_family_indices.transfer_family, this->queue_family_indices.compute_family};
        std::vector<uint32_t> culled_buffer_queue_families = {
            this->queue_family_indices.compute_family, this->queue_family_indices.graphics_family};
        std::vector<uint32_t> simulation_data_queue_families = {
            this->queue_family_indices.transfer_family, this->queue_family_indices.graphics_family};
        this->position_staging_buffers[in_flight_frame].reset();
        this->position_staging_buffers[in_flight_frame] =
            this->mem_allocator.template allocate_buffer<bounding_box_t>(
                this->buffer_capacities[in_flight_frame],
                vk::BufferUsageFlagBits::eStorageBuffer | vk::BufferUsageFlagBits::eTransferSrc,
                this->queue_family_indices.transfer_family,
                true,
                VMA_MEMORY_USAGE_AUTO_PREFER_HOST);
        bit::set_name(this->device,
                      this->position_staging_buffers[in_flight_frame]->buffer,
                      "position-staging-buffer-" + std::to_string(in_flight_frame));
        bit::set_name(this->device,
                      this->position_staging_buffers[in_flight_frame]->allocation->
                      GetMemory(),
                      "position-staging-buffer-memory-" + std::to_string(in_flight_frame));

        this->position_culling_buffers[in_flight_frame].reset();
        this->position_culling_buffers[in_flight_frame] =
            this->mem_allocator.allocate_buffer<bounding_box_t>(
                this->buffer_capacities[in_flight_frame],
                vk::BufferUsageFlagBits::eStorageBuffer | vk::BufferUsageFlagBits::eTransferDst,
                culling_buffer_queue_families,
                false);
        bit::set_name(this->device,
                      this->position_culling_buffers[in_flight_frame]->buffer,
                      "position-culling-buffer-" + std::to_string(in_flight_frame));
        bit::set_name(this->device,
                      this->position_culling_buffers[in_flight_frame]->allocation->
                      GetMemory(),
                      "position-culling-buffer-memory-" + std::to_string(in_flight_frame));

        this->culled_indices_buffers[in_flight_frame].reset();
        this->culled_indices_buffers[in_flight_frame] = this->mem_allocator.allocate_buffer<
            uint32_t>(
            this->buffer_capacities[in_flight_frame],
            vk::BufferUsageFlagBits::eStorageBuffer,
            culled_buffer_queue_families,
            false);
        bit::set_name(this->device,
                      this->culled_indices_buffers[in_flight_frame]->buffer,
                      "culled-indices-buffer-" + std::to_string(in_flight_frame));
        bit::set_name(this->device,
                      this->culled_indices_buffers[in_flight_frame]->allocation->
                      GetMemory(),
                      "culled-indices-buffer-memory-" + std::to_string(in_flight_frame));

        this->simulation_data_staging_buffers[in_flight_frame].reset();
        this->simulation_data_staging_buffers[in_flight_frame] =
            this->mem_allocator.template allocate_buffer<T>(
                this->buffer_capacities[in_flight_frame],
                vk::BufferUsageFlagBits::eStorageBuffer | vk::BufferUsageFlagBits::eTransferSrc,
                this->queue_family_indices.transfer_family,
                true,
                VMA_MEMORY_USAGE_AUTO_PREFER_HOST);
        bit::set_name(this->device,
                      this->simulation_data_staging_buffers[in_flight_frame]->buffer,
                      "simulation-data-staging-buffer-" + std::to_string(in_flight_frame));
        bit::set_name(this->device,
                      this->simulation_data_staging_buffers[in_flight_frame]->allocation->
                      GetMemory(),
                      "simulation-data-staging-buffer-memory-" + std::to_string(in_flight_frame));

        this->simulation_data_buffers[in_flight_frame].reset();
        this->simulation_data_buffers[in_flight_frame] = this->mem_allocator.allocate_buffer<T>(
            this->buffer_capacities[in_flight_frame],
            vk::BufferUsageFlagBits::eStorageBuffer | vk::BufferUsageFlagBits::eTransferDst,
            simulation_data_queue_families,
            false);
        bit::set_name(this->device,
                      this->simulation_data_buffers[in_flight_frame]->buffer,
                      "simulation-data-buffer-" + std::to_string(in_flight_frame));
        bit::set_name(this->device,
                      this->simulation_data_buffers[in_flight_frame]->allocation->
                      GetMemory(),
                      "simulation-data-buffer-memory-" + std::to_string(in_flight_frame));
    }

    void write_descriptor_sets() {
        for (int i = 0; i < FRAMES_IN_FLIGHT; ++i) {
            this->write_descriptor_set(i);
        }
    }

    void write_descriptor_set(uint32_t in_flight_frame) {
        std::vector<vk::WriteDescriptorSet> descriptor_set_writes{};

        // Compute
        vk::DescriptorBufferInfo culling_buffer_info{
            this->position_culling_buffers[in_flight_frame]->buffer, 0,
            vk::WholeSize};
        vk::WriteDescriptorSet write_culling_descriptor_set{
            *this->compute_descriptor_sets[in_flight_frame],
            0, 0, vk::DescriptorType::eStorageBuffer,
            {}, culling_buffer_info, {}
        };
        descriptor_set_writes.emplace_back(write_culling_descriptor_set);
        vk::DescriptorBufferInfo culled_buffer_info{
            this->culled_indices_buffers[in_flight_frame]->buffer, 0,
            vk::WholeSize};
        vk::WriteDescriptorSet write_culled_descriptor_set{
            *this->compute_descriptor_sets[in_flight_frame],
            1, 0, vk::DescriptorType::eStorageBuffer,
            {}, culled_buffer_info, {}
        };
        descriptor_set_writes.emplace_back(write_culled_descriptor_set);
        vk::DescriptorBufferInfo cmd_buffer_info{
            this->compute_indirect_command_buffers[in_flight_frame]->buffer, 0,
            vk::WholeSize};
        vk::WriteDescriptorSet write_cmd_descriptor_set{
            *this->compute_descriptor_sets[in_flight_frame],
            2, 0, vk::DescriptorType::eStorageBuffer,
            {}, cmd_buffer_info, {}
        };
        descriptor_set_writes.emplace_back(write_cmd_descriptor_set);

        // Graphics
        vk::WriteDescriptorSet write_positions_descriptor_set{
            *this->graphics_descriptor_sets[in_flight_frame],
            0, 0, vk::DescriptorType::eStorageBuffer,
            {}, culling_buffer_info, {}
        };
        descriptor_set_writes.emplace_back(write_positions_descriptor_set);
        vk::DescriptorBufferInfo sim_data_buffer_info{
            this->simulation_data_buffers[in_flight_frame]->buffer, 0,
            vk::WholeSize};
        vk::WriteDescriptorSet write_sim_data_descriptor_set{
            *this->graphics_descriptor_sets[in_flight_frame],
            1, 0, vk::DescriptorType::eStorageBuffer,
            {}, sim_data_buffer_info, {}
        };
        descriptor_set_writes.emplace_back(write_sim_data_descriptor_set);

        this->device.updateDescriptorSets(descriptor_set_writes, {});
    }

    vk::Device &device;
    bit::queue_family_indices_t &queue_family_indices;
    bit::memory_allocator &mem_allocator;
    entt::storage<bounding_box_t> &position_storage;
    entt::storage<T> &simulation_data_storage;

    std::vector<vk::UniqueDescriptorSet> compute_descriptor_sets;
    std::vector<vk::UniqueDescriptorSet> graphics_descriptor_sets;

    std::vector<size_t> buffer_capacities;

    std::vector<UniqueVmaBuffer> compute_indirect_command_buffers;
    std::vector<UniqueVmaBuffer> position_staging_buffers;
    std::vector<UniqueVmaBuffer> position_culling_buffers;
    std::vector<UniqueVmaBuffer> culled_indices_buffers;

    std::vector<UniqueVmaBuffer> simulation_data_staging_buffers;
    std::vector<UniqueVmaBuffer> simulation_data_buffers;

    bit::pipeline &culling_pipeline;
    bit::pipeline &reset_culling_pipeline;
    bit::pipeline &graphics_pipeline;
    vk::Extent2D &extent;
};

#endif //BUILDIT_TEST_CHIP_BUFFER_HPP