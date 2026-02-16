cmake_minimum_required(VERSION 3.25 FATAL_ERROR)

# Build badver test library as a shared library
add_library(test_badver SHARED badver.c)

# Set output name to badver.poe
set_target_properties(test_badver PROPERTIES
    OUTPUT_NAME "badver"
    PREFIX ""
    SUFFIX ".poe"
)

# Set position independent code (required for shared libraries)
set_target_properties(test_badver PROPERTIES
    POSITION_INDEPENDENT_CODE ON
)

# Include directories
target_include_directories(test_badver PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../../src/inc"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../include"
)

# Link against libpoco
target_link_libraries(test_badver PRIVATE libpoco)

# For Windows, symbols are exported via __declspec(dllexport) in source
if(WIN32)
    set_target_properties(test_badver PROPERTIES
        WINDOWS_EXPORT_ALL_SYMBOLS FALSE
    )
else()
    # Set visibility defaults for Linux/macOS
    target_compile_options(test_badver PRIVATE -fvisibility=default)
endif()

INSTALL(TARGETS test_badver DESTINATION ${CMAKE_INSTALL_PREFIX}/tests/)

