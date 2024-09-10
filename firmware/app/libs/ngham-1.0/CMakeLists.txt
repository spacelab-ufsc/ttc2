cmake_minimum_required(VERSION 3.0)

# Disable in-source builds to prevent source tree corruption.
if(" ${CMAKE_SOURCE_DIR}" STREQUAL " ${CMAKE_BINARY_DIR}")
	message(FATAL_ERROR "FATAL: In-source builds are not allowed. You should create a separate directory for build files.")
endif()

project(ngham C)

add_subdirectory(rsclib)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/rsclib/include)


add_library(ngham STATIC ${CMAKE_CURRENT_SOURCE_DIR}/src/ngham.c ${CMAKE_CURRENT_SOURCE_DIR}/src/ngham_packets.c ${CMAKE_CURRENT_SOURCE_DIR}/src/crc_ccitt.c ${CMAKE_CURRENT_SOURCE_DIR}/src/ccsds_scrambler.c ${CMAKE_CURRENT_SOURCE_DIR}/src/ngham_packets.c)

install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/ DESTINATION include)
install(TARGETS ngham DESTINATION lib)
