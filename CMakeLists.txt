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

add_subdirectory(lib/imgui)
set(SKIP_PERFORMANCE_COMPARISON ON) # We don't need Boost (Required by cereal for performance comparison)
set(BUILD_SANDBOX OFF) # We don't need Sandbox tests (Warning because of SHARED Library with WebAssembly)
set(ECS_HISTORY_SHARED ON)
add_subdirectory(lib/ecs_history)

set(CONFIG_DIR ${CMAKE_BINARY_DIR}/config)
set(RUN_CONFIG_DIR ${CMAKE_BINARY_DIR}/run/config)
install(DIRECTORY ${CONFIG_DIR} DESTINATION ${CMAKE_INSTALL_DATADIR}/${CMAKE_PROJECT_NAME})

add_library(BuildIT_moduleapi INTERFACE)
target_include_directories(BuildIT_moduleapi INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(BuildIT_moduleapi INTERFACE glm::glm EnTT::EnTT ecs_history fmt)

set(MODULES_INSTALL_DIR ${CMAKE_INSTALL_DATADIR}/${CMAKE_PROJECT_NAME}/modules)
add_subdirectory(modules/simulation)
add_subdirectory(modules/plugin)
add_subdirectory(modules/gui)
add_subdirectory(modules/network)

add_executable(BuildIT src/buildit.cpp)
target_link_libraries(BuildIT CLI11::CLI11 BuildIT_moduleapi ecs_history)
target_compile_definitions(BuildIT PRIVATE SPDLOG_HEADER_ONLY)
install(TARGETS BuildIT)

add_custom_command(TARGET BuildIT POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:BuildIT> ${CMAKE_BINARY_DIR}/run/
)


if (UNIX AND NOT APPLE)
    set(CPACK_APPIMAGE_DESKTOP_FILE "buildit.desktop")
    set(CPACK_PACKAGE_ICON ${PROJECT_BINARY_DIR}/assets/buildit.png)
    configure_file(assets/buildit.desktop ${PROJECT_BINARY_DIR}/assets/buildit.desktop)
    configure_file(assets/buildit.png ${PROJECT_BINARY_DIR}/assets/buildit.png)
    install(FILES ${PROJECT_BINARY_DIR}/assets/buildit.desktop DESTINATION ${CMAKE_INSTALL_DATADIR}/applications)
    install(FILES ${PROJECT_BINARY_DIR}/assets/buildit.png DESTINATION ${CMAKE_INSTALL_DATADIR}/icons/hicolor/128x128/apps)
endif ()

set(CPACK_GENERATOR DEB)
set(CPACK_PACKAGE_DESCRIPTION "Logic gate simulation game")
set(CPACK_DEBIAN_PACKAGE_SHLIBDEPS ON)
set(CPACK_PACKAGE_CONTACT "Felix Weglehner")
include(CPack)