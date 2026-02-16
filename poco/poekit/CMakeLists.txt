cmake_minimum_required(VERSION 3.16 FATAL_ERROR)

## hello library example
add_subdirectory(hello)

## color conversion utilities.
add_subdirectory(colorutl)

## postage stamp library (used by flisumry.poc)
add_subdirectory(pstamp)

## Add test libraries for error state testing
add_subdirectory(test_libs/noentry)
add_subdirectory(test_libs/nullret)
add_subdirectory(test_libs/badver)
add_subdirectory(test_libs/empty)

