cmake_minimum_required(VERSION 3.16 FATAL_ERROR)

project(animhost C)

add_library(animhost SHARED
    hostlib_table.c
)

set_target_properties(animhost PROPERTIES
    POSITION_INDEPENDENT_CODE ON
    C_VISIBILITY_PRESET hidden
    VISIBILITY_INLINES_HIDDEN YES
)

target_include_directories(animhost
    PUBLIC
        ${CMAKE_SOURCE_DIR}/src/inc
)

if (UNIX)
    target_link_libraries(animhost m)
endif()

if (CMAKE_GENERATOR STREQUAL "Xcode")
    # Xcode bundle specifics not required for a plain dylib
else()
    INSTALL(TARGETS animhost DESTINATION ./)
endif()


