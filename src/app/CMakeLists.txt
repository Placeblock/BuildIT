qt_add_executable(BuildIT
        main.cpp
)

target_link_libraries(BuildIT PRIVATE Qt6::Widgets model)

set_target_properties(BuildIT PROPERTIES
        WIN32_EXECUTABLE ON
        MACOSX_BUNDLE ON
)