cmake_minimum_required(VERSION 3.16)

find_package(glfw3 QUIET)
if (NOT glfw3)
    FetchContent_Declare(
            glfw3
            GIT_REPOSITORY https://github.com/glfw/glfw.git
            GIT_TAG 3.4
    )
    FetchContent_MakeAvailable(glfw3)
endif ()
find_package(glm QUIET)
if (NOT glm)
    FetchContent_Declare(
            glm
            GIT_REPOSITORY https://github.com/g-truc/glm.git
            GIT_TAG 1.0.1
    )
    FetchContent_MakeAvailable(glm)
endif ()

find_package(VulkanMemoryAllocator QUIET)
if (NOT VulkanMemoryAllocator)
    FetchContent_Declare(
            VulkanMemoryAllocator
            GIT_REPOSITORY https://github.com/GPUOpen-LibrariesAndSDKs/VulkanMemoryAllocator.git
            GIT_TAG v3.3.0
    )
    FetchContent_MakeAvailable(VulkanMemoryAllocator)
endif ()

find_package(Vulkan REQUIRED)
find_program(glslc_executable NAMES glslc HINTS Vulkan::glslc PATHS "${VULKAN_SDK}/bin")

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
install(DIRECTORY ${SHADER_BINARY_DIR}/
        DESTINATION ${CMAKE_INSTALL_DATADIR}/${CMAKE_PROJECT_NAME}/shaders
        FILES_MATCHING PATTERN "*.spv"
)

add_executable(BuildIT_gui)
add_executable(BuildIT::GUI ALIAS BuildIT_gui)
file(GLOB_RECURSE SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp ${CMAKE_CURRENT_SOURCE_DIR}/src/*.c)
file(GLOB_RECURSE HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/include/*.h ${CMAKE_CURRENT_SOURCE_DIR}/include/*.hpp)
target_sources(BuildIT_gui PRIVATE ${SOURCES} ${HEADERS})
target_include_directories(BuildIT_gui PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

add_dependencies(BuildIT_gui shaders)

target_link_libraries(BuildIT_gui PUBLIC glfw glm::glm Vulkan::Vulkan spdlog::spdlog imgui GPUOpen::VulkanMemoryAllocator EnTT::EnTT)

install(TARGETS BuildIT_gui)