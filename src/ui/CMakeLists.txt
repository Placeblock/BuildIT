cmake_minimum_required(VERSION 3.16)

FetchContent_Declare(
        glfw3
        GIT_REPOSITORY https://github.com/glfw/glfw.git
        GIT_TAG 3.4
)
FetchContent_MakeAvailable(glfw3)
FetchContent_Declare(
        glm
        GIT_REPOSITORY https://github.com/g-truc/glm.git
        GIT_TAG 1.0.1
)
FetchContent_MakeAvailable(glm)
find_package(Vulkan REQUIRED)
find_program(glslc_executable NAMES glslc HINTS Vulkan::glslc PATHS "${VULKAN_SDK}/bin")

FetchContent_Declare(
        VulkanMemoryAllocator
        GIT_REPOSITORY https://github.com/GPUOpen-LibrariesAndSDKs/VulkanMemoryAllocator.git
        GIT_TAG v3.3.0
)
FetchContent_MakeAvailable(VulkanMemoryAllocator)

set(SHADER_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/shaders)
set(SHADER_BINARY_DIR ${CMAKE_CURRENT_BINARY_DIR}/shaders)
file(GLOB SHADERS
        ${SHADER_SOURCE_DIR}/*.vert
        ${SHADER_SOURCE_DIR}/*.frag
        ${SHADER_SOURCE_DIR}/*.comp
)
add_custom_target(create_shader_binary_dir
        COMMAND ${CMAKE_COMMAND} -E make_directory ${SHADER_BINARY_DIR}
        COMMENT "Creating shader binary directory"
)
foreach (SOURCE IN LISTS SHADERS)
    get_filename_component(FILENAME ${SOURCE} NAME)
    add_custom_command(
            COMMAND
            ${glslc_executable} -o ${SHADER_BINARY_DIR}/${FILENAME}.spv ${SOURCE}
            OUTPUT ${SHADER_BINARY_DIR}/${FILENAME}.spv
            DEPENDS ${SOURCE}
            COMMENT "Compiling Shader ${FILENAME}"
    )
    list(APPEND SPV_SHADERS ${SHADER_BINARY_DIR}/${FILENAME}.spv)
endforeach ()

add_custom_target(shaders ALL DEPENDS ${SPV_SHADERS})
add_dependencies(shaders create_shader_binary_dir)

add_executable(ui src/application.cpp src/chip_buffer.hpp
        include/bit/shader.hpp
        include/bit/queue_family.hpp
        include/bit/debug-names.hpp
        include/bit/memory/memory_allocator.hpp
        include/bit/pipeline/build_pipeline.hpp
        include/bit/pipeline/compute_pipeline_builder.hpp
        include/bit/pipeline/descriptor_set_layout_builder.hpp
        include/bit/pipeline/graphics_pipeline_builder.hpp
        src/bit/memory/memory_allocator.cpp
        src/bit/pipeline/compute_pipeline_builder.cpp
        src/bit/pipeline/graphics_pipeline_builder.cpp
        src/bit/pipeline/descriptor_set_layout_builder.cpp
)
add_dependencies(ui shaders)

target_include_directories(ui PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(ui PUBLIC glfw glm::glm Vulkan::Vulkan spdlog::spdlog imgui GPUOpen::VulkanMemoryAllocator EnTT::EnTT)
target_include_directories(ui PUBLIC ${CMAKE_SOURCE_DIR}/lib)