cmake_minimum_required(VERSION 3.16)

project(BuildIT VERSION 0.0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)
include(GNUInstallDirs)

find_package(spdlog QUIET)
if (NOT spdlog_FOUND)
    set(SPDLOG_BUILD_SHARED ON)
    set(SPDLOG_INSTALL ON)
    FetchContent_Declare(
            spdlog
            GIT_REPOSITORY https://github.com/gabime/spdlog.git
            GIT_TAG v1.15.3
    )
    FetchContent_MakeAvailable(spdlog)
endif ()

find_package(fmt QUIET)
if (NOT fmt_FOUND)
    FetchContent_Declare(fmt
            GIT_REPOSITORY https://github.com/fmtlib/fmt.git
            GIT_TAG master
    )
endif ()

find_package(CLI11 QUIET)
if (NOT CLI11_FOUND)
    FetchContent_Declare(
            CLI11
            GIT_REPOSITORY https://github.com/CLIUtils/CLI11.git
            GIT_TAG v2.5.0
    )
    FetchContent_MakeAvailable(CLI11)
endif ()


set(GLM_BUILD_INSTALL OFF)
find_package(glm QUIET)
if (NOT glm_FOUND)
    FetchContent_Declare(
            glm
            GIT_REPOSITORY https://github.com/g-truc/glm.git
            GIT_TAG 1.0.1
    )
    FetchContent_MakeAvailable(glm)
endif ()

configure_file(environment.hpp.in include/environment.hpp @ONLY)
include_directories(${CMAKE_CURRENT_BINARY_DIR}/include)

add_subdirectory(lib/ecs_history)

add_executable(BuildIT src/buildit.cpp
        include/buildit/chips.hpp)
target_include_directories(BuildIT PUBLIC include/)
target_link_libraries(BuildIT CLI11::CLI11 ecs_history)
install(TARGETS BuildIT)

file(GLOB_RECURSE SOURCES "${PROJECT_SOURCE_DIR}/src/*.cpp")
target_sources(BuildIT PRIVATE ${SOURCES})


add_custom_command(TARGET BuildIT POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:BuildIT> ${CMAKE_BINARY_DIR}/run/
)