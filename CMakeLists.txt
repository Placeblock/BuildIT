cmake_minimum_required(VERSION 3.16)

project(BuildIT VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets)
qt_standard_project_setup()

add_subdirectory(lib/entt)

set(SKIP_PERFORMANCE_COMPARISON ON) # We don't need Boost (Required by cereal for performance comparison)
set(BUILD_SANDBOX OFF) # We don't need Sandbox tests (Warning because of SHARED Library with Webassembly)
add_subdirectory(lib/cereal)

add_subdirectory(src/app)
add_subdirectory(src/model)
add_subdirectory(src/simulation)

target_link_libraries(BuildIT PRIVATE EnTT::EnTT cereal::cereal model simulation)