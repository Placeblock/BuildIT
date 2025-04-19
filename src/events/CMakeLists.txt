add_library(events STATIC
        event.hpp
        eventdispatcher.hpp
        history.hpp
)
target_include_directories(events INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})