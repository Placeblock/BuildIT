add_library(plugin_loader STATIC
        include/buildit/loader/api.h
        src/api.cpp
        include/buildit/loader/loader.h
        src/loader.cpp
)
set_target_properties(plugin_loader PROPERTIES LINKER_LANGUAGE CXX)
target_link_libraries(plugin_loader plugin_api simulation)
target_include_directories(plugin_loader PUBLIC include)