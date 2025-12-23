cmake_minimum_required(VERSION 3.16)


set(CPPZMQ_BUILD_TESTS OFF)
set(CPPZMQ_BUILD_EXAMPLES OFF)
option(CPPZMQ_BUILD_TESTS OFF)
FetchContent_Declare(
        cppzmq
        GIT_REPOSITORY https://github.com/zeromq/cppzmq
        GIT_TAG v4.11.0
)
FetchContent_MakeAvailable(cppzmq)


add_library(ecs STATIC
        include/ecs/registry_node.hpp)
set_target_properties(ecs PROPERTIES LINKER_LANGUAGE CXX)
target_include_directories(ecs PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(ecs PUBLIC common simulation spdlog::spdlog ecs_net cppzmq)