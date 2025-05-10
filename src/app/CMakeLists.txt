cmake_minimum_required(VERSION 3.16)

find_package(Qt6 COMPONENTS Quick Gui REQUIRED)
qt_standard_project_setup(REQUIRES 6.5)

qt_add_executable(app main.cpp
        ../history/include/history/worldhistory.hpp)

add_subdirectory(qml/application)

target_link_libraries(app PRIVATE model Qt6::Gui Qt6::Quick)

set_target_properties(app PROPERTIES
        WIN32_EXECUTABLE ON
        MACOSX_BUNDLE ON
)