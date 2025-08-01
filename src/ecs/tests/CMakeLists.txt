add_executable(test_model test_model.cpp)
target_link_libraries(test_model PUBLIC ecs EnTT::EnTT)

add_test(NAME ModelTests COMMAND test_model)