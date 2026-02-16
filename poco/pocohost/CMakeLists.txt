cmake_minimum_required(VERSION 3.16 FATAL_ERROR)

project(pocohost C)

add_library(pocohost SHARED
    pjhost.c
)

set_target_properties(pocohost PROPERTIES
    POSITION_INDEPENDENT_CODE ON
    C_VISIBILITY_PRESET hidden
    VISIBILITY_INLINES_HIDDEN YES
)

target_include_directories(pocohost
    PRIVATE
        ${CMAKE_SOURCE_DIR}/src/inc
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/../include
)

if (UNIX)
    target_link_libraries(pocohost m)
endif()

if (CMAKE_GENERATOR STREQUAL "Xcode")
    # Xcode bundle specifics not required for a plain dylib
else()
    INSTALL(TARGETS pocohost DESTINATION ./)
endif()


