add_library(model STATIC)

target_link_libraries(model INTERFACE simulation)
target_include_directories(model PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
