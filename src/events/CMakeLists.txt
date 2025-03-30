qt_add_library(events STATIC
        models.h
)
target_include_directories(events INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})