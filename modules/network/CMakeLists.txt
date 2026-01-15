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

add_library(BuildIT_Network STATIC
        include/buildit/network/network.hpp
        src/network.cpp
        include/buildit/network/commit_message.hpp)
set_target_properties(BuildIT_Network PROPERTIES LINKER_LANGUAGE CXX)
target_include_directories(BuildIT_Network PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(BuildIT_Network PUBLIC spdlog::spdlog ecs_history cppzmq)

add_library(BuildIT::Network ALIAS BuildIT_Network)