add_library(controller STATIC
        controller.hpp
)
target_include_directories(controller INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})