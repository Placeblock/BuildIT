cmake_minimum_required(VERSION 3.16)

add_library(history STATIC)
set_target_properties(history PROPERTIES LINKER_LANGUAGE CXX)

target_sources(history PRIVATE
        src/history/event.cpp
        include/history/event.hpp
        src/history/history.cpp
        include/history/history.hpp
        include/history/eventdispatcher.hpp
        src/history/eventtarget.cpp
        include/history/eventtarget.hpp)

target_include_directories(history PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(history PRIVATE EnTT::EnTT spdlog::spdlog model)

add_subdirectory(tests)
