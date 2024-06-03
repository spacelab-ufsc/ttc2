cmake_minimum_required(VERSION 3.0)

project(ngham_test C)

include_directories(${CMAKE_SOURCE_DIR}/../include)

add_library(ngham STATIC ${CMAKE_SOURCE_DIR}/../src/ngham.c)

add_executable(ngham_test ${CMAKE_SOURCE_DIR}/test.c)

target_link_libraries(ngham_test cmocka)
target_link_libraries(ngham_test ngham)
