cmake_minimum_required(VERSION 3.16 FATAL_ERROR)

project(poco LANGUAGES C)

set (CMAKE_C_STANDARD 99)

if(CMAKE_BUILD_TYPE STREQUAL "debug")
	add_compile_definitions(-DDEVELOPMENT=1 -DDEBUG=1)
endif()


set(LIBPOCO_SRC_FILES
		bcutil.c
		bop.c
		chopper.c
		code.c
		declare.c
		dummylib.c
		pocodos_standalone.c
		fold.c
		funccall.c
		linklist.c
		mathlib.c
		pocmemry.c
		poco.c
		pocodis.c
		pocoface.c
		pocolib.c
		pocoload.c
		pocoop.c
		pocotype.c
		postring.c
		poco_ffi.c
		pp.c
		ppeval.c
		runops.c
		safefile.c
		statemen.c
		strlib.c
		struct.c
		token.c
		trace.c
		varinit.c

		lostdio.h
		poco.h
		pocoop.h
		pp.h
		pplists.h
		statemen.h
		struct.h
		)

list(TRANSFORM
		LIBPOCO_SRC_FILES
	PREPEND
		"${CMAKE_CURRENT_SOURCE_DIR}/src/"
)

add_library(libpoco ${LIBPOCO_SRC_FILES})
set_target_properties(libpoco PROPERTIES PREFIX "")
# Put PRIVATE src/inc first in search order, then PUBLIC poco/include
# Do NOT include Animator's headers by default to avoid shadowing system headers
target_include_directories(libpoco
    PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include"
)

# Prefer external host for pj_* (provided by pocohost)
target_compile_definitions(libpoco PUBLIC USE_EXTERNAL_PJ_HOST)

# Host helpers for Poco CLI and external apps (must be before libpoco/poco)
add_subdirectory(pocohost)

target_link_libraries(libpoco PUBLIC ffi_static hashmap PRIVATE pocohost trdfile)

set(POCO_MAIN_SRC_FILES "${CMAKE_CURRENT_SOURCE_DIR}/src/main.c")
if (APPLE OR UNIX)
	list(APPEND POCO_MAIN_SRC_FILES "${CMAKE_CURRENT_SOURCE_DIR}/src/poco_unix.c")
endif()

add_executable(poco ${POCO_MAIN_SRC_FILES})
target_compile_definitions(poco PRIVATE USE_EXTERNAL_PJ_HOST)
target_link_libraries(poco libpoco pocohost trdfile trdutil ffi_static hashmap)

# Ensure poco can find libpocohost at runtime
if(APPLE)
	set_target_properties(poco PROPERTIES
		INSTALL_RPATH "@loader_path"
	)
elseif(UNIX)
	set_target_properties(poco PROPERTIES
		INSTALL_RPATH "$ORIGIN"
	)
endif()

INSTALL(TARGETS poco DESTINATION ./)

# NOTE: poekit (POE modules) is added from the root CMakeLists.txt
# AFTER src's resource directory install, so native .poe files are not
# overwritten by the resource directory copy.

# Add test subdirectory
add_subdirectory(test)

