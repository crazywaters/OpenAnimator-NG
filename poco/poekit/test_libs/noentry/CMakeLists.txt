cmake_minimum_required(VERSION 3.25 FATAL_ERROR)

# Build noentry test library as a shared library
add_library(test_noentry SHARED noentry.c)

# Set output name to noentry.poe
set_target_properties(test_noentry PROPERTIES
    OUTPUT_NAME "noentry"
    PREFIX ""
    SUFFIX ".poe"
)

# Set position independent code (required for shared libraries)
set_target_properties(test_noentry PROPERTIES
    POSITION_INDEPENDENT_CODE ON
)

# For Windows, export all symbols
if(WIN32)
    set_target_properties(test_noentry PROPERTIES
        WINDOWS_EXPORT_ALL_SYMBOLS TRUE
    )
else()
    # Set visibility defaults for Linux/macOS
    target_compile_options(test_noentry PRIVATE -fvisibility=default)
endif()

INSTALL(TARGETS test_noentry DESTINATION ${CMAKE_INSTALL_PREFIX}/tests/)

