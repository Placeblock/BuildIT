add_executable(test_controller test_controller.cpp)
target_link_libraries(test_controller PRIVATE events worlds EnTT::EnTT)

add_test(NAME ControllerTests COMMAND test_controller)