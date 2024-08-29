cmake_minimum_required(VERSION 3.0)

project(rsc_test C)

include_directories(${CMAKE_SOURCE_DIR}/../include)

add_library(rsc STATIC ${CMAKE_SOURCE_DIR}/../src/rsc.c)

add_executable(rsc_test ${CMAKE_SOURCE_DIR}/test.c)

target_link_libraries(rsc_test cmocka)
target_link_libraries(rsc_test rsc)
