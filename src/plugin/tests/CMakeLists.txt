add_executable(test_plugin test_plugin.cpp)
target_link_libraries(test_plugin PRIVATE simulation plugin_api_cpp)

add_test(NAME PluginTests COMMAND test_plugin)

add_dependencies(test_plugin plugin_default)