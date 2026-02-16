set(REXLIB_SRC_FILES
	rexhost/keytext.c
	rexhost/rexload.c
	rexhost/rexlib.c

	aasyslib.c
	stdiolib.c

	rexhost/rexload.h
	)

add_library(minirex ${REXLIB_SRC_FILES})
# Use src/inc/ as canonical headers (rexlib/inc/ is deprecated)
target_include_directories(minirex PUBLIC "${CMAKE_SOURCE_DIR}/src/inc")

