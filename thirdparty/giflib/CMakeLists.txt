cmake_minimum_required(VERSION 3.25 FATAL_ERROR)

project(giflib)

if (UNIX)
	set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wno-format-truncation")
endif(UNIX)

set(GIFLIB_SOURCES
	dgif_lib.c
	egif_lib.c
	gifalloc.c
	gif_err.c
	gif_font.c
	gif_hash.c
	openbsd-reallocarray.c

	gif_hash.h
	gif_lib.h
	gif_lib_private.h
)

add_library(giflib STATIC ${GIFLIB_SOURCES})
target_include_directories(giflib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

