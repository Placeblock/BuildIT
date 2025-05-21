cmake_minimum_required(VERSION 3.16)

add_library(model)
set_target_properties(model PROPERTIES LINKER_LANGUAGE CXX)

target_sources(model PRIVATE
        include/model/collision.hpp
        include/model/components.hpp
        src/model/components.cpp
        src/model/collision.cpp
        include/model/globalentitymixin.hpp
        include/model/changemixin.hpp
        include/model/snapshot.hpp
        src/model/snapshot.cpp)

target_include_directories(model PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(model PUBLIC common simulation EnTT::EnTT spdlog::spdlog Bitsery::bitsery)

add_subdirectory(tests)
