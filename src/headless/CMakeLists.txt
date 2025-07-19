cmake_minimum_required(VERSION 3.16)

add_executable(headless main.cpp)

target_link_libraries(headless PRIVATE CLI11::CLI11 spdlog::spdlog EnTT::EnTT)