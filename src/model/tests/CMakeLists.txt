add_executable(test_model test_model.cpp)
target_link_libraries(test_model PRIVATE model flecs::flecs_static)

add_test(NAME ModelTests COMMAND test_model)