cmake_minimum_required(VERSION 3.25 FATAL_ERROR)

# Include the Poco POE library helper
list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/poco/cmake")
include(PocoPoeLibrary)

# Build hello.poe using the helper function
# The helper automatically:
#   - Creates a shared library with .poe extension
#   - Sets up include paths to src/inc and poco/include
#   - Installs library to INSTALL_DIR
#   - Installs and registers the test file
add_poe_library(hello
    SOURCES hello.c
    DEPS libpoco
    INSTALL_DIR ${CMAKE_INSTALL_PREFIX}/tests
    TEST_FILE hello.poc
)

# For Windows, symbols are exported via __declspec(dllexport) in source
if(WIN32)
    set_target_properties(hello PROPERTIES
        WINDOWS_EXPORT_ALL_SYMBOLS FALSE
    )
else()
    # Set visibility defaults for Linux/macOS
    target_compile_options(hello PRIVATE -fvisibility=default)
endif()
