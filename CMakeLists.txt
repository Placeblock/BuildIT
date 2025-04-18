cmake_minimum_required(VERSION 3.16)

project(BuildIT VERSION 1.0.0 LANGUAGES CXX)

enable_testing()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets)
qt_standard_project_setup()

set(FLECS_SHARED OFF) # We want to use flecs as a static Library (WebAssembly support)
add_subdirectory(lib/flecs)

set(SKIP_PERFORMANCE_COMPARISON ON) # We don't need Boost (Required by cereal for performance comparison)
set(BUILD_SANDBOX OFF) # We don't need Sandbox tests (Warning because of SHARED Library with WebAssembly)
add_subdirectory(lib/cereal)

add_subdirectory(src/simulation)
add_subdirectory(src/model)
add_subdirectory(src/events)
add_subdirectory(src/controller)
add_subdirectory(src/app)