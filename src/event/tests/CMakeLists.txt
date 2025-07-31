add_executable(test_history test_history.cpp)
target_link_libraries(test_history PRIVATE event EnTT::EnTT)

add_test(NAME HistoryTests COMMAND test_history)