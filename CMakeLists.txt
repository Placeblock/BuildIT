cmake_minimum_required(VERSION 3.16)

project(BuildIT VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)

FetchContent_Declare(
        spdlog
        GIT_REPOSITORY https://github.com/gabime/spdlog.git
        GIT_TAG v1.15.3
)

FetchContent_MakeAvailable(spdlog)

#cmake_policy(SET CMP0077 NEW)
#set(FLECS_SHARED OFF) # We want to use flecs as a static Library (WebAssembly support)
#add_subdirectory(lib/flecs)

#set(SKIP_PERFORMANCE_COMPARISON ON) # We don't need Boost (Required by cereal for performance comparison)
#set(BUILD_SANDBOX OFF) # We don't need Sandbox tests (Warning because of SHARED Library with WebAssembly)
#add_subdirectory(lib/cereal)

FetchContent_Declare(
        CLI11
        GIT_REPOSITORY https://github.com/CLIUtils/CLI11.git
        GIT_TAG v2.5.0
)
FetchContent_MakeAvailable(CLI11)

enable_testing()

add_subdirectory(lib/imgui)
add_subdirectory(lib/ecs_history)
add_subdirectory(src/common)
add_subdirectory(src/simulation)
add_subdirectory(src/simulation/tests)
add_subdirectory(src/ecs)
add_subdirectory(src/ecs/tests)
add_subdirectory(src/host)
add_subdirectory(src/plugin)
add_subdirectory(src/ui)
add_subdirectory(src/headless)