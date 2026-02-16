cmake_minimum_required(VERSION 3.25 FATAL_ERROR)

# Build nullret test library as a shared library
add_library(test_nullret SHARED nullret.c)

# Set output name to nullret.poe
set_target_properties(test_nullret PROPERTIES
    OUTPUT_NAME "nullret"
    PREFIX ""
    SUFFIX ".poe"
)

# Set position independent code (required for shared libraries)
set_target_properties(test_nullret PROPERTIES
    POSITION_INDEPENDENT_CODE ON
)

# Include directories
target_include_directories(test_nullret PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../../src/inc"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../include"
)

# Link against libpoco
target_link_libraries(test_nullret PRIVATE libpoco)

# For Windows, symbols are exported via __declspec(dllexport) in source
if(WIN32)
    set_target_properties(test_nullret PROPERTIES
        WINDOWS_EXPORT_ALL_SYMBOLS FALSE
    )
else()
    # Set visibility defaults for Linux/macOS
    target_compile_options(test_nullret PRIVATE -fvisibility=default)
endif()

INSTALL(TARGETS test_nullret DESTINATION ${CMAKE_INSTALL_PREFIX}/tests/)

