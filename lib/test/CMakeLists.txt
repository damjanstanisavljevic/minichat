cmake_minimum_required(VERSION 3.10)
project(minichat LANGUAGES CXX)

add_compile_options(-Wall -Wextra -Werror -pedantic -std=c++17 -g)

include_directories(../src/)

find_package(GTest)

add_executable(testsuite ${CMAKE_SOURCE_DIR}/
    test_request/test_basic_request.cpp
    test_request/test_construct_request.cpp
    test_request/test_parser_request.cpp
    test_response/test_basic_response.cpp
    test_response/test_construct_response.cpp
    test_response/test_parser_response.cpp
    testsuite.cpp
)

target_link_libraries(testsuite gtest)
target_link_libraries(testsuite gtest_main)
target_link_libraries(testsuite pthread)
