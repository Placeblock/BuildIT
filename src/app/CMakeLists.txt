cmake_minimum_required(VERSION 3.16)

find_package(glfw3 REQUIRED)
find_package(glm REQUIRED)
find_package(Vulkan REQUIRED)

add_executable(app main.cpp
        ../plugin/include/plugin/plugin.hpp)
target_link_libraries(app PUBLIC glfw glm::glm Vulkan::Vulkan)
