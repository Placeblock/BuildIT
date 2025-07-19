cmake_minimum_required(VERSION 3.16)

find_package(glfw3 REQUIRED)
find_package(glm REQUIRED)
find_package(Vulkan REQUIRED)

add_executable(app src/main.cpp
        include/app/vulkan/circuitboard.hpp
        src/app/vulkan/circuitboard.cpp
        include/app/vulkan/circuitboard_manager.hpp
        src/app/vulkan/circuitboard_manager.cpp
        include/app/vulkan/vulkancontext.hpp
        src/app/vulkan/vulkancontext.cpp
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

target_include_directories(app PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include ../../lib/imgui ../../lib/imgui/backends)
target_link_libraries(app PUBLIC glfw glm::glm Vulkan::Vulkan spdlog::spdlog)
