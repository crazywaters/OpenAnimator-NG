cmake_minimum_required(VERSION 3.25 FATAL_ERROR)

# Build empty test library as a shared library
add_library(test_empty SHARED empty.c)

# Set output name to empty.poe
set_target_properties(test_empty PROPERTIES
    OUTPUT_NAME "empty"
    PREFIX ""
    SUFFIX ".poe"
)

# Set position independent code (required for shared libraries)
set_target_properties(test_empty PROPERTIES
    POSITION_INDEPENDENT_CODE ON
)

# Include directories
target_include_directories(test_empty PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../../src/inc"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../include"
)

# Link against libpoco
target_link_libraries(test_empty PRIVATE libpoco)

# For Windows, symbols are exported via __declspec(dllexport) in source
if(WIN32)
    set_target_properties(test_empty PROPERTIES
        WINDOWS_EXPORT_ALL_SYMBOLS FALSE
    )
else()
    # Set visibility defaults for Linux/macOS
    target_compile_options(test_empty PRIVATE -fvisibility=default)
endif()

INSTALL(TARGETS test_empty DESTINATION ${CMAKE_INSTALL_PREFIX}/tests/)

