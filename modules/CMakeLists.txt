cmake_minimum_required(VERSION 3.16)

add_library(BuildIT_moduleapi INTERFACE)
target_include_directories(BuildIT_moduleapi INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(BuildIT_moduleapi INTERFACE glm::glm EnTT::EnTT ecs_history)