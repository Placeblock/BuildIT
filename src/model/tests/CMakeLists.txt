add_executable(test_model test_model.cpp)
target_link_libraries(test_model PRIVATE model EnTT::EnTT)

add_test(NAME ModelTests COMMAND test_model)