cmake_minimum_required(VERSION 3.16)

#FetchContent_Declare(
#        glfw3
#        GIT_REPOSITORY https://github.com/glfw/glfw.git
#        GIT_TAG 3.4
#)
#FetchContent_MakeAvailable(glfw3)
#FetchContent_Declare(
#        glm
#        GIT_REPOSITORY https://github.com/g-truc/glm.git
#        GIT_TAG 1.0.1
#)
#FetchContent_MakeAvailable(glm)

find_package(Vulkan REQUIRED)

add_library(imgui
        imgui.cpp
        imgui.h
        imgui_demo.cpp
        imgui_tables.cpp
        imgui_draw.cpp
        imgui_widgets.cpp
        backends/imgui_impl_vulkan.h
        backends/imgui_impl_glfw.h
        backends/imgui_impl_vulkan.cpp
        backends/imgui_impl_glfw.cpp
)

target_link_libraries(imgui PUBLIC Vulkan::Vulkan)
target_include_directories(imgui PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/backends)