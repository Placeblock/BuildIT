add_library(model STATIC
        components.hpp
        storage/serialization.hpp
)
target_include_directories(model INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
