
set(MINIZ_FILES
	miniz.c
	miniz.h
)

add_library(miniz STATIC ${MINIZ_FILES})
target_include_directories(miniz PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
