cmake_minimum_required(VERSION 3.16)

set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,--verbose")

find_package(glfw3 REQUIRED)
find_package(glm REQUIRED)
find_package(Vulkan REQUIRED glslang)

add_executable(app main.cpp)
link_directories(/opt/vulkan-sdk/1.4.313.0/lib)
target_link_libraries(app PUBLIC glfw glm::glm Vulkan::Vulkan spdlog::spdlog Vulkan::glslang)
