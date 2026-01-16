cmake_minimum_required(VERSION 3.16)

project(BuildIT VERSION 0.0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if (NOT DISABLE_FETCH_CONTENT)
    include(FetchContent)
endif ()
include(GNUInstallDirs)

set(SPDLOG_BUILD_SHARED ON)
set(SPDLOG_INSTALL ON)
find_library(spdlog QUIET)
if (NOT spdlog)
    FetchContent_Declare(
            spdlog
            GIT_REPOSITORY https://github.com/gabime/spdlog.git
            GIT_TAG v1.15.3
    )
    FetchContent_MakeAvailable(spdlog)
endif ()

find_library(CLI11 QUIET)
if (NOT CLI11)
    FetchContent_Declare(
            CLI11
            GIT_REPOSITORY https://github.com/CLIUtils/CLI11.git
            GIT_TAG v2.5.0
    )
    FetchContent_MakeAvailable(CLI11)
endif ()

#enable_testing()
if (DEFINED ENV{CONFIG_DIR})
    set(CONFIG_DIR ENV{CONFIG_DIR})
else ()
    if (UNIX)
        if (DEFINED ENV{XDG_CONFIG_HOME})
            set(CONFIG_DIR $ENV{XDG_CONFIG_HOME}/${CMAKE_PROJECT_NAME})
        else ()
            set(CONFIG_DIR $ENV{HOME}/.config/${CMAKE_PROJECT_NAME})
        endif ()
    elseif (WIN32)
        set(CONFIG_DIR ${LOCALAPPDATA}/${CMAKE_PROJECT_NAME})
    endif ()
endif ()
configure_file(environment.hpp.in include/environment.hpp @ONLY)
include_directories(${CMAKE_CURRENT_BINARY_DIR}/include)

add_subdirectory(lib/imgui)
set(SKIP_PERFORMANCE_COMPARISON ON) # We don't need Boost (Required by cereal for performance comparison)
set(BUILD_SANDBOX OFF) # We don't need Sandbox tests (Warning because of SHARED Library with WebAssembly)
add_subdirectory(lib/ecs_history)

add_subdirectory(modules/)
set(MODULES_INSTALL_DIR ${CMAKE_INSTALL_DATADIR}/${CMAKE_PROJECT_NAME}/modules)

add_subdirectory(modules/simulation)
add_subdirectory(modules/plugin)
add_subdirectory(modules/gui)
add_subdirectory(modules/network)

add_executable(BuildIT buildit.cpp)
target_link_libraries(BuildIT CLI11::CLI11 BuildIT_moduleapi spdlog::spdlog)

install(TARGETS BuildIT)

set(CPACK_GENERATOR AppImage NSIS DEB)
set(CPACK_PACKAGE_DESCRIPTION "Logic gate simulation game")
set(CPACK_DEBIAN_PACKAGE_SHLIBDEPS ON)
set(CPACK_PACKAGE_CONTACT "Felix Weglehner")
include(CPack)