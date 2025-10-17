cmake_minimum_required(VERSION 3.16 FATAL_ERROR)

# Function to add a positive test (expects successful execution)
# Usage: add_poco_pos_test(test_name relative_path_to_poc_file)
function(add_poco_pos_test TEST_NAME POC_FILE)
    set(TEST_FILE "${CMAKE_CURRENT_SOURCE_DIR}/${POC_FILE}")
    add_test(
        NAME "poco_pos_${TEST_NAME}"
        COMMAND $<TARGET_FILE:poco> "${TEST_FILE}"
        WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
    )
    # Positive tests should succeed (return 0)
    set_tests_properties("poco_pos_${TEST_NAME}" PROPERTIES
        PASS_REGULAR_EXPRESSION "Success|success|hi|Hello"
        TIMEOUT 10
    )
endfunction()

# Function to add a negative test (expects compilation/runtime error)
# Usage: add_poco_neg_test(test_name relative_path_to_poc_file expected_error_pattern)
function(add_poco_neg_test TEST_NAME POC_FILE)
    set(TEST_FILE "${CMAKE_CURRENT_SOURCE_DIR}/${POC_FILE}")
    add_test(
        NAME "poco_neg_${TEST_NAME}"
        COMMAND $<TARGET_FILE:poco> "${TEST_FILE}"
        WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
    )
    # Negative tests should fail (non-zero return code or error message)
    set_tests_properties("poco_neg_${TEST_NAME}" PROPERTIES
        WILL_FAIL TRUE
        TIMEOUT 10
    )
endfunction()

# Function to add a general poco test with custom properties
# Usage: add_poco_test(test_name relative_path_to_poc_file)
function(add_poco_test TEST_NAME POC_FILE)
    set(TEST_FILE "${CMAKE_CURRENT_SOURCE_DIR}/${POC_FILE}")
    add_test(
        NAME "poco_${TEST_NAME}"
        COMMAND $<TARGET_FILE:poco> "${TEST_FILE}"
        WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
    )
    set_tests_properties("poco_${TEST_NAME}" PROPERTIES
        TIMEOUT 10
    )
endfunction()


###############################################################################
# Positive Tests - These should compile and run successfully
###############################################################################

add_poco_pos_test(array01 "pos/PARRAY01.POC")
add_poco_pos_test(array02 "pos/PARRAY02.POC")
add_poco_pos_test(array03 "pos/PARRAY03.POC")
add_poco_pos_test(chcon01 "pos/PCHCON01.POC")
add_poco_pos_test(for01 "pos/PFOR01.POC")
add_poco_pos_test(if01 "pos/PIF01.POC")
add_poco_pos_test(lonesem "pos/PLONESEM.POC")
add_poco_pos_test(struc01 "pos/PSTRUC01.POC")
add_poco_pos_test(struc02 "pos/PSTRUC02.POC")
add_poco_pos_test(struc03 "pos/PSTRUC03.POC")
add_poco_pos_test(tok01 "pos/PTOK01.POC")


###############################################################################
# Negative Tests - These should fail to compile or produce errors
###############################################################################

add_poco_neg_test(amper01 "neg/NAMPER01.POC")
add_poco_neg_test(amper02 "neg/NAMPER02.POC")
add_poco_neg_test(amper03 "neg/NAMPER03.POC")
add_poco_neg_test(amper04 "neg/NAMPER04.POC")
add_poco_neg_test(arrfunc "neg/NARRFUNC.POC")
add_poco_neg_test(arrvar "neg/NARRVAR.POC")
add_poco_neg_test(break01 "neg/NBREAK01.POC")
add_poco_neg_test(break02 "neg/NBREAK02.POC")
add_poco_neg_test(cast01 "neg/NCAST01.POC")
add_poco_neg_test(dolla01 "neg/NDOLLA01.POC")
add_poco_neg_test(dolla02 "neg/NDOLLA02.POC")
add_poco_neg_test(dolla03 "neg/NDOLLA03.POC")
add_poco_neg_test(dolla04 "neg/NDOLLA04.POC")
add_poco_neg_test(funcvar "neg/NFUNCVAR.POC")
add_poco_neg_test(noelse "neg/NOELSE.POC")
add_poco_neg_test(noif "neg/NOIF.POC")
add_poco_neg_test(parmct0 "neg/NPARMCT0.POC")
add_poco_neg_test(parmct1 "neg/NPARMCT1.POC")
add_poco_neg_test(parmct2 "neg/NPARMCT2.POC")
add_poco_neg_test(parmct3 "neg/NPARMCT3.POC")
add_poco_neg_test(parmct4 "neg/NPARMCT4.POC")
add_poco_neg_test(recurse "neg/NRECURSE.POC")
add_poco_neg_test(squot01 "neg/NSQUOT01.POC")
add_poco_neg_test(squot02 "neg/NSQUOT02.POC")
add_poco_neg_test(strng01 "neg/NSTRNG01.POC")
add_poco_neg_test(strng02 "neg/NSTRNG02.POC")
add_poco_neg_test(strng03 "neg/NSTRNG03.POC")
add_poco_neg_test(struc01 "neg/NSTRUC01.POC")
add_poco_neg_test(struc02 "neg/NSTRUC02.POC")
add_poco_neg_test(struc03 "neg/NSTRUC03.POC")
add_poco_neg_test(struc04 "neg/NSTRUC04.POC")
add_poco_neg_test(struc05 "neg/NSTRUC05.POC")
add_poco_neg_test(struc06 "neg/NSTRUC06.POC")
add_poco_neg_test(struc07 "neg/NSTRUC07.POC")
add_poco_neg_test(struc08 "neg/NSTRUC08.POC")
add_poco_neg_test(struc09 "neg/NSTRUC09.POC")
add_poco_neg_test(struc10 "neg/NSTRUC10.POC")
add_poco_neg_test(struc11 "neg/NSTRUC11.POC")
add_poco_neg_test(struc12 "neg/NSTRUC12.POC")
add_poco_neg_test(struc13 "neg/NSTRUC13.POC")
add_poco_neg_test(struc14 "neg/NSTRUC14.POC")
add_poco_neg_test(tokcmnt "neg/NTOKCMNT.POC")
add_poco_neg_test(tokone "neg/NTOKONE.POC")
add_poco_neg_test(tokpp "neg/NTOKPP.POC")
add_poco_neg_test(tokzero "neg/NTOKZERO.POC")
add_poco_neg_test(type01 "neg/NTYPE01.POC")
add_poco_neg_test(type02 "neg/NTYPE02.POC")
add_poco_neg_test(varfunc "neg/NVARFUNC.POC")
add_poco_neg_test(voidret "neg/NVOIDRET.POC")


###############################################################################
# General Tests - Various poco scripts in the root test directory
###############################################################################

add_poco_test(hi "HI.POC")
add_poco_test(array "ARRAY.POC")
add_poco_test(ffi_01 "po_ffi_01.poc")
# Add more general tests as needed:
# add_poco_test(atof "ATOF.POC")
# add_poco_test(comma "COMMA.POC")
# add_poco_test(float "FLOAT.POC")
# add_poco_test(forever "FOREVER.POC")
# add_poco_test(if "IF.POC")
# add_poco_test(short "SHORT.POC")
# add_poco_test(struct "STRUCT.POC")
# etc...

