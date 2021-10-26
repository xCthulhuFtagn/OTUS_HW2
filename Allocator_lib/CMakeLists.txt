cmake_minimum_required(VERSION 3.0)

project(ALLOCATOR VERSION 0.0.1)

add_library(A_lib STATIC Allocator.h)

set_target_properties(
    A_lib PROPERTIES
    CXX_VERSION 11
    CXX_VERSION_REQUIRED ON
    LINKER_LANGUAGE CXX
)

target_include_directories(
    A_lib
    PRIVATE
    {CMAKE_CURRENT_BINARY_DIR}
)