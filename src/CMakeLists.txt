
include_directories(${root_SOURCE_DIR}/include)
add_library(circus_client client.cpp json11.cpp circus_exception.cpp)

