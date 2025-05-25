cmake_minimum_required(VERSION 3.16)

project(BuildIT VERSION 1.0.0 LANGUAGES CXX)

enable_testing()

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(spdlog REQUIRED)

#cmake_policy(SET CMP0077 NEW)
#set(FLECS_SHARED OFF) # We want to use flecs as a static Library (WebAssembly support)
#add_subdirectory(lib/flecs)
add_subdirectory(lib/entt)

#set(SKIP_PERFORMANCE_COMPARISON ON) # We don't need Boost (Required by cereal for performance comparison)
#set(BUILD_SANDBOX OFF) # We don't need Sandbox tests (Warning because of SHARED Library with WebAssembly)
#add_subdirectory(lib/cereal)

add_subdirectory(lib/bitsery)
add_subdirectory(lib/CLI11)

add_subdirectory(src/common)
add_subdirectory(src/simulation)
add_subdirectory(src/model)
add_subdirectory(src/serialization)
add_subdirectory(src/event)
add_subdirectory(src/plugin)
add_subdirectory(src/plugin/plugins/default)
add_subdirectory(src/app)
add_subdirectory(src/headless)
