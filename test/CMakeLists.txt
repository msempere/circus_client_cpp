include_directories(${root_SOURCE_DIR}/include)
add_executable(test test.cpp)
target_link_libraries(test circus_client zmq)
