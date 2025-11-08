add_executable(test_plugin test_plugin.cpp)
target_link_libraries(test_plugin PRIVATE simulation plugin_loader plugin_api_cpp)

add_test(NAME PluginTests COMMAND test_plugin)

add_dependencies(test_plugin plugin_default)

add_custom_command(TARGET test_plugin POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E make_directory $<TARGET_FILE_DIR:test_plugin>/plugins
)
add_custom_command(TARGET test_plugin POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:plugin_default> $<TARGET_FILE_DIR:test_plugin>/plugins/
)