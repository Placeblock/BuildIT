cmake_minimum_required(VERSION 3.16)

find_package(glfw3 REQUIRED)
find_package(glm REQUIRED)
find_package(Vulkan REQUIRED)
find_program(glslc_executable NAMES glslc HINTS Vulkan::glslc)

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

add_executable(app src/main.cpp
        include/app/vulkan/circuitboard.hpp
        src/app/vulkan/circuitboard.cpp
        include/app/vulkan/circuitboard_manager.hpp
        src/app/vulkan/circuitboard_manager.cpp
        include/app/vulkan/vulkancontext.hpp
        src/app/vulkan/vulkancontext.cpp
        include/app/vulkan/imgui_circuitboard.hpp
        src/app/vulkan/imgui_circuitboard.cpp
        ../../lib/imgui/imgui.h
        ../../lib/imgui/imgui.cpp
        ../../lib/imgui/imgui_demo.cpp
        ../../lib/imgui/imgui_tables.cpp
        ../../lib/imgui/imgui_draw.cpp
        ../../lib/imgui/imgui_widgets.cpp
        ../../lib/imgui/backends/imgui_impl_vulkan.h
        ../../lib/imgui/backends/imgui_impl_glfw.h
        ../../lib/imgui/backends/imgui_impl_vulkan.cpp
        ../../lib/imgui/backends/imgui_impl_glfw.cpp)
add_dependencies(app shaders)

target_include_directories(app PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include ../../lib/imgui ../../lib/imgui/backends)
target_link_libraries(app PUBLIC glfw glm::glm Vulkan::Vulkan spdlog::spdlog)
