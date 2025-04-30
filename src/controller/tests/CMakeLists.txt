add_executable(test_controller test_controller.cpp)
target_link_libraries(test_controller PRIVATE events worlds flecs::flecs_static)

add_test(NAME ControllerTests COMMAND test_controller)