qt_add_library(simulation STATIC
        Node.h Node.cpp
)
target_include_directories(simulation INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})