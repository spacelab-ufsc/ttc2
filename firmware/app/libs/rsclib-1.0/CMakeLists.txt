cmake_minimum_required(VERSION 3.0)

# Disable in-source builds to prevent source tree corruption.
if(" ${CMAKE_SOURCE_DIR}" STREQUAL " ${CMAKE_BINARY_DIR}")
	message(FATAL_ERROR "FATAL: In-source builds are not allowed. You should create a separate directory for build files.")
endif()

project(rsc LANGUAGES C VERSION 0.1.0 HOMEPAGE_URL https://github.com/mgm8/rsclib)

include_directories(${CMAKE_SOURCE_DIR}/include)

add_library(rsc STATIC ${CMAKE_SOURCE_DIR}/src/rsc.c)

install(DIRECTORY ${CMAKE_SOURCE_DIR}/include/ DESTINATION include)
install(TARGETS rsc DESTINATION lib)
