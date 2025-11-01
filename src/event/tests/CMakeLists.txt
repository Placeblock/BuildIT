add_executable(test_history test_history.cpp)
target_link_libraries(test_history PRIVATE event EnTT::EnTT)

add_test(NAME HistoryTests COMMAND test_history)


add_executable(test_events test_events.cpp)
target_link_libraries(test_events PRIVATE event EnTT::EnTT)

add_test(NAME EventTests COMMAND test_events)