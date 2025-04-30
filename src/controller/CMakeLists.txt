cmake_minimum_required(VERSION 3.16)

#add_library(controller)
#set_target_properties(controller PROPERTIES LINKER_LANGUAGE CXX)

#target_include_directories(controller PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

#target_link_libraries(controller PRIVATE simulation model flecs::flecs_static events)

add_subdirectory(tests)