# cmake/PocoPoeLibrary.cmake
#
# Usage:
#   add_poe_library(
#       <target>                 # logical target name
#       SOURCES file1.c ...       # required – source files
#       [INCLUDES dir1 dir2 ...]  # optional – additional include paths
#       [DEPS lib1 lib2 ...]      # optional – link libraries
#       [INSTALL_DIR <path>]      # optional – defaults to ${CMAKE_INSTALL_PREFIX}/poco
#       [TEST_FILE <path>]        # optional – .poc file to register as a test
#       [SCRIPTS file1.poc ...]   # optional – .poc scripts to install (without test)
#   )
#
# Example:
#   add_poe_library(hello
#       SOURCES hello.c
#       DEPS libpoco
#       TEST_FILE hello.poc
#       SCRIPTS helper1.poc helper2.poc
#   )
#
# After calling this function you get:
#   • A shared-library target that produces <target>.poe (not .so/.dll)
#   • Automatic RPATH handling so the .poe finds Animator Pro libs
#   • Automatic install rules:
#       - Library → INSTALL_DIR (default: ${CMAKE_INSTALL_PREFIX}/poco)
#       - Test file → ${CMAKE_INSTALL_PREFIX}/tests (if TEST_FILE specified)
#       - Scripts → ${CMAKE_INSTALL_PREFIX}/tests (if SCRIPTS specified)
#   • A CTest entry named 'poco_<target>' if TEST_FILE is provided
#

function(add_poe_library TARGET)
    cmake_parse_arguments(
        POE
        ""                                      # no boolean options
        "INSTALL_DIR;TEST_FILE;RUNNER"          # single-value keywords
        "SOURCES;INCLUDES;DEPS;SCRIPTS"         # multi-value keywords
        ${ARGN}
    )

    if(NOT POE_SOURCES)
        message(FATAL_ERROR "add_poe_library(${TARGET}): SOURCES is required")
    endif()

    # ----------------------------------------------------------------------
    # 1. Normal shared-library target
    # ----------------------------------------------------------------------
    add_library(${TARGET} SHARED ${POE_SOURCES})

    # ----------------------------------------------------------------------
    # 2. Make it a '.poe' rather than '.so' / '.dll'
    # ----------------------------------------------------------------------
    set_target_properties(${TARGET} PROPERTIES
        PREFIX ""                 # no 'lib' prefix → hello.poe, not libhello.poe
        OUTPUT_NAME ${TARGET}
        SUFFIX ".poe"
    )

    # ----------------------------------------------------------------------
    # 3. Include directories & link libraries
    # ----------------------------------------------------------------------
    target_include_directories(${TARGET}
        PRIVATE
            ${POE_INCLUDES}
            ${CMAKE_SOURCE_DIR}/src/inc            # Animator public headers
            ${CMAKE_SOURCE_DIR}/poco/include       # Poco public headers
    )
    if(POE_DEPS)
        target_link_libraries(${TARGET} PRIVATE ${POE_DEPS})
    endif()

    # ----------------------------------------------------------------------
    # 4. rpath - so at runtime Animator finds its deps
    # ----------------------------------------------------------------------
    if(APPLE)
        set_target_properties(${TARGET} PROPERTIES
            INSTALL_RPATH "@loader_path;@loader_path/.."
        )
    elseif(UNIX)
        set_target_properties(${TARGET} PROPERTIES
            INSTALL_RPATH "$ORIGIN"
        )
    endif()

    # ----------------------------------------------------------------------
    # 5. Install rules
    # ----------------------------------------------------------------------
    if(NOT POE_INSTALL_DIR)
        set(POE_INSTALL_DIR "${CMAKE_INSTALL_PREFIX}/poco")
    endif()

    install(TARGETS ${TARGET}
        DESTINATION ${POE_INSTALL_DIR}
    )
    # Also drop a copy next to the test script so dlopen finds libanimhost via @loader_path/..
    if(POE_TEST_FILE)
        install(TARGETS ${TARGET}
            DESTINATION ${CMAKE_INSTALL_PREFIX}/tests)
    endif()

    # ----------------------------------------------------------------------
    # 6. Optional: Install scripts to tests directory
    # ----------------------------------------------------------------------
    if(POE_SCRIPTS)
        foreach(script ${POE_SCRIPTS})
            install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/${script} 
                    DESTINATION ${CMAKE_INSTALL_PREFIX}/tests)
        endforeach()
    endif()

    # ----------------------------------------------------------------------
    # 7. Optional: Register test file and create CTest entry
    # ----------------------------------------------------------------------
    if(POE_TEST_FILE)
        # Get just the filename for the installed location
        get_filename_component(_test_filename ${POE_TEST_FILE} NAME)
        
        # Install the test file
        install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/${POE_TEST_FILE} 
                DESTINATION ${CMAKE_INSTALL_PREFIX}/tests)
        
        # Determine which runner to use (default: poco)
        if(NOT POE_RUNNER)
            set(POE_RUNNER "poco")
        endif()
        
        # Create CTest entry with appropriate runner
        if(POE_RUNNER STREQUAL "ani")
            # Use Animator for tests that require Animator features
            add_test(
                NAME "poco_${TARGET}"
                COMMAND ${CMAKE_INSTALL_PREFIX}/ani -poc ${CMAKE_INSTALL_PREFIX}/tests/${_test_filename}
                WORKING_DIRECTORY ${CMAKE_INSTALL_PREFIX}
            )
        else()
            # Use standalone poco (default)
        add_test(
            NAME "poco_${TARGET}"
            COMMAND ${CMAKE_INSTALL_PREFIX}/poco ${CMAKE_INSTALL_PREFIX}/tests/${_test_filename}
            WORKING_DIRECTORY ${CMAKE_INSTALL_PREFIX}
        )
        endif()
    endif()
endfunction()

