
if (UNIX)
	set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wno-missing-field-initializers")
endif()

set(HOST_SRC_FILES
	host/picdrive.c
	host/pdr_support.c
)

set(FLILO_SRC_FILES
	flilo/compbrun.c
	flilo/flicomp.c
	flilo/flilo.c
)

set(SDLPDR_SRC_FILES
	sdl_pdr.c
	sdl_pdr.h

	sdl_bmp.c
	sdl_pcx.c
)

add_library(picdrive
	${HOST_SRC_FILES}
	${FLILO_SRC_FILES}
	${SDLPDR_SRC_FILES}
#	${SDLIMAGE_SRC_FILES}
)

target_include_directories(
	picdrive
	PUBLIC
		../inc
		../pj_sdl
)
target_link_libraries(picdrive flilib)
