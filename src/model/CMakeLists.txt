qt_add_library(model STATIC
        models.h
)
target_include_directories(model INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})