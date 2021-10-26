cmake_minimum_required(VERSION 3.0)

project(DONUT VERSION 0.0.1)

add_library(D_lib STATIC Donut.h)

set_target_properties(
    D_lib PROPERTIES
    CXX_VERSION 11
    CXX_VERSION_REQUIRED ON
    LINKER_LANGUAGE CXX
)

target_include_directories(
    D_lib
    PRIVATE
    {CMAKE_CURRENT_BINARY_DIR}
)