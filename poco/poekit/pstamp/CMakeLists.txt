cmake_minimum_required(VERSION 3.25 FATAL_ERROR)

# Include the Poco POE library helper
list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/poco/cmake")
include(PocoPoeLibrary)

# Build pstamp.poe — postage stamp image reduction library
# Used by flisumry.poc (installed in the resource directory alongside this .poe)
add_poe_library(pstamp
    SOURCES pstamp.c
    DEPS libpoco animhost raster gfxlib trdutil
    INSTALL_DIR ${CMAKE_INSTALL_PREFIX}/resource
)
