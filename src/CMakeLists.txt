cmake_minimum_required(VERSION 3.0)

project(MAIN VERSION 0.0.1)

add_executable(main main.cpp)

set_target_properties(
    main PROPERTIES
    CXX_VERSION 11
    CXX_VERSION_REQUIREMENTS ON
)

target_include_directories(
    main
    PRIVATE
    ${CMAKE_CURRENT_BINARY_DIR}
)

target_include_directories(
    main
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/../Allocator_lib
)

target_include_directories(
    main
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/../Donut_lib
)

target_link_libraries(main D_lib A_lib)
