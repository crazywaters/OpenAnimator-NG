cmake_minimum_required(VERSION 3.25 FATAL_ERROR)

# Include the Poco POE library helper
list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/poco/cmake")
include(PocoPoeLibrary)

# Build colorutl.poe using the helper function
# The helper automatically:
#   - Creates a shared library with .poe extension
#   - Sets up include paths to src/inc and poco/include
#   - Installs library to INSTALL_DIR
#   - Installs and registers the test file
add_poe_library(colorutl
    SOURCES colorutl.c rgbhls.c
    DEPS libpoco animhost
    INSTALL_DIR ${CMAKE_INSTALL_PREFIX}/resource
    TEST_FILE coltest.poc
)

