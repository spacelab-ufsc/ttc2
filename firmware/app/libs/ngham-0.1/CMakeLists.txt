cmake_minimum_required(VERSION 3.0)

# Disable in-source builds to prevent source tree corruption.
if(" ${CMAKE_SOURCE_DIR}" STREQUAL " ${CMAKE_BINARY_DIR}")
	message(FATAL_ERROR "FATAL: In-source builds are not allowed. You should create a separate directory for build files.")
endif()

project(ngham C)

include_directories(${CMAKE_SOURCE_DIR}/include)

add_library(ngham STATIC ${CMAKE_SOURCE_DIR}/src/ngham.c)
add_library(crc_ccitt STATIC ${CMAKE_SOURCE_DIR}/src/crc_ccitt.c)

install(DIRECTORY ${CMAKE_SOURCE_DIR}/include/ DESTINATION include)
install(TARGETS ngham DESTINATION lib)
