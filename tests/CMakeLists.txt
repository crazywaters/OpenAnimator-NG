
cmake_minimum_required(VERSION 3.16)
project(TestApp)

set(CMAKE_C_STANDARD 11)

# set(SDL_TEST_SOURCES
# 		sdl_test.c
# 		../src/pj_sdl/pj_sdl.c
# )

# add_executable(sdl_test "${SDL_TEST_SOURCES}")
# target_include_directories(sdl_test PUBLIC ../src/inc)
# target_link_libraries(sdl_test
# 		SDL3::SDL3-static
# 		m
# 		nfd
# 		"-framework AppKit"
# 		"-framework UniformTypeIdentifiers"
# )

# set_target_properties(
# 		sdl_test
# 	PROPERTIES
# 		MACOSX_BUNDLE YES
# 		XCODE_ATTRIBUTE_CLANG_ENABLE_OBJC_ARC YES
# 		XCODE_ATTRIBUTE_PRODUCT_BUNDLE_IDENTIFIER "com.vpaint.test-app"
# )

# ## pre-build required or signing will fail
# add_custom_command(
# 	TARGET sdl_test
# 	PRE_BUILD
# 		COMMAND ${CMAKE_COMMAND} -E copy_directory
# 			"${CMAKE_CURRENT_SOURCE_DIR}/resource" "$<TARGET_FILE_DIR:sdl_test>/../Resources"
# )


## ffi test (disabled; ffi_test.cc kept for reference)
# if (WITH_POCO)
# 	set(FFI_TEST_SOURCES
#             ffi_test.cc
#     )
#
# 	add_executable(ffi_test "${FFI_TEST_SOURCES}")
# 	target_link_libraries(ffi_test ffi_static)
# endif()

###############################################################################
# Minunit C unit tests for Poco library functions
# Runs only when WITH_POCO=ON (consistent with "Poco library testing" scope)
###############################################################################
if (WITH_POCO)
	# Minunit is header-only
	set(MINUNIT_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/thirdparty/minunit")

	# --- fnsplit tests ---
	add_executable(test_fnsplit test_fnsplit.c)
	target_include_directories(test_fnsplit PRIVATE
		${MINUNIT_INCLUDE_DIR}
		${CMAKE_SOURCE_DIR}/src/inc
	)
	target_link_libraries(test_fnsplit PRIVATE trdfile)
	add_test(NAME test_fnsplit COMMAND test_fnsplit)
	set_tests_properties(test_fnsplit PROPERTIES TIMEOUT 30)

	# --- color HLS tests ---
	add_executable(test_color_hls test_color_hls.c)
	target_include_directories(test_color_hls PRIVATE
		${MINUNIT_INCLUDE_DIR}
		${CMAKE_SOURCE_DIR}/src/inc
	)
	target_link_libraries(test_color_hls PRIVATE gfxlib trdutil)
	add_test(NAME test_color_hls COMMAND test_color_hls)
	set_tests_properties(test_color_hls PROPERTIES TIMEOUT 30)
endif()
