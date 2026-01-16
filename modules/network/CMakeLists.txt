cmake_minimum_required(VERSION 3.16)

find_package(cppzmq QUIET)
if (NOT cppzmq_FOUND)
    set(CPPZMQ_BUILD_TESTS OFF)
    set(CPPZMQ_BUILD_EXAMPLES OFF)
    option(CPPZMQ_BUILD_TESTS OFF)
    FetchContent_Declare(
            cppzmq
            GIT_REPOSITORY https://github.com/zeromq/cppzmq
            GIT_TAG v4.11.0
    )
    FetchContent_MakeAvailable(cppzmq)
endif ()

add_library(BuildIT_network SHARED src/module.cpp
        include/buildit/network/history.hpp)
add_library(BuildIT::Network ALIAS BuildIT_network)
file(GLOB_RECURSE SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp ${CMAKE_CURRENT_SOURCE_DIR}/src/*.c)
file(GLOB_RECURSE HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/include/*.h ${CMAKE_CURRENT_SOURCE_DIR}/include/*.hpp)
target_sources(BuildIT_network PRIVATE ${SOURCES} ${HEADERS})
target_include_directories(BuildIT_network PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

target_link_libraries(BuildIT_network PUBLIC spdlog::spdlog ecs_history cppzmq BuildIT_moduleapi)

install(TARGETS BuildIT_network DESTINATION ${MODULES_INSTALL_DIR})
configure_file(default-config.ini ${CONFIG_DIR}/network.ini)
configure_file(default-config.ini ${RUN_CONFIG_DIR}/network.ini)

add_custom_command(TARGET BuildIT_network POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:BuildIT_network> ${CMAKE_BINARY_DIR}/run/modules/$<TARGET_FILE_NAME:BuildIT_network>
)