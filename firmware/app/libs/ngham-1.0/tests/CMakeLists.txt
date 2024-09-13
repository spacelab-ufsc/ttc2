cmake_minimum_required(VERSION 3.0)

project(ngham_test C)

include_directories(${CMAKE_SOURCE_DIR}/../include)
include_directories(${CMAKE_SOURCE_DIR}/../rsclib/include)

add_subdirectory(${CMAKE_SOURCE_DIR}/.. ${CMAKE_BINARY_DIR}/../build_ngham)

add_executable(ngham_test ${CMAKE_SOURCE_DIR}/test.c)

target_link_libraries(ngham_test cmocka ngham rsc)