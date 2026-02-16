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

add_poco_pos_test(array_1d "pos/array_1d.poc")
add_poco_pos_test(array_2d "pos/array_2d.poc")
add_poco_pos_test(array_max_dims "pos/array_max_dims.poc")
add_poco_pos_test(char_constants "pos/char_constants.poc")
add_poco_pos_test(fnsplit_roundtrip "pos/fnsplit_roundtrip.poc")
add_poco_pos_test(for_nested "pos/for_nested.poc")
add_poco_pos_test(if_nested "pos/if_nested.poc")
add_poco_pos_test(lone_semicolons "pos/lone_semicolons.poc")
add_poco_pos_test(pointer_return "pos/pointer_return.poc")
add_poco_pos_test(sprintf_variadic "pos/sprintf_variadic.poc")
add_poco_pos_test(struct_nested "pos/struct_nested.poc")
add_poco_pos_test(struct_semicolons "pos/struct_semicolons.poc")
add_poco_pos_test(struct_union_enum "pos/struct_union_enum.poc")
add_poco_pos_test(token_long_idents "pos/token_long_idents.poc")


###############################################################################
# Negative Tests - These should fail to compile or produce errors
###############################################################################

add_poco_neg_test(addr_dot_ampersand "neg/addr_dot_ampersand.poc")
add_poco_neg_test(addr_dot_deref "neg/addr_dot_deref.poc")
add_poco_neg_test(addr_in_decl "neg/addr_in_decl.poc")
add_poco_neg_test(addr_of_addr "neg/addr_of_addr.poc")
add_poco_neg_test(array_as_number "neg/array_as_number.poc")
add_poco_neg_test(call_array_as_func "neg/call_array_as_func.poc")
add_poco_neg_test(call_int_as_func "neg/call_int_as_func.poc")
add_poco_neg_test(cast_ptr_to_func "neg/cast_ptr_to_func.poc")
add_poco_neg_test(char_in_param_list "neg/char_in_param_list.poc")
add_poco_neg_test(char_to_ptr "neg/char_to_ptr.poc")
add_poco_neg_test(dollar_in_name_01 "neg/dollar_in_name_01.poc")
add_poco_neg_test(dollar_in_name_02 "neg/dollar_in_name_02.poc")
add_poco_neg_test(dollar_in_name_03 "neg/dollar_in_name_03.poc")
add_poco_neg_test(dollar_in_name_04 "neg/dollar_in_name_04.poc")
add_poco_neg_test(else_no_body "neg/else_no_body.poc")
add_poco_neg_test(expect_bool_expr "neg/expect_bool_expr.poc")
add_poco_neg_test(func_as_number "neg/func_as_number.poc")
add_poco_neg_test(if_no_body "neg/if_no_body.poc")
add_poco_neg_test(param_count_0_of_1 "neg/param_count_0_of_1.poc")
add_poco_neg_test(param_count_1_of_2 "neg/param_count_1_of_2.poc")
add_poco_neg_test(param_count_2_of_3 "neg/param_count_2_of_3.poc")
add_poco_neg_test(param_count_3_of_4 "neg/param_count_3_of_4.poc")
add_poco_neg_test(param_count_4_of_5 "neg/param_count_4_of_5.poc")
add_poco_neg_test(stack_overflow "neg/stack_overflow.poc")
add_poco_neg_test(string_multiline_02 "neg/string_multiline_02.poc")
add_poco_neg_test(string_multiline_03 "neg/string_multiline_03.poc")
add_poco_neg_test(string_unterminated "neg/string_unterminated.poc")
add_poco_neg_test(struct_arrow_on_val "neg/struct_arrow_on_val.poc")
add_poco_neg_test(struct_bad_indirect "neg/struct_bad_indirect.poc")
add_poco_neg_test(struct_bad_member "neg/struct_bad_member.poc")
add_poco_neg_test(struct_as_var_name "neg/struct_as_var_name.poc")
add_poco_neg_test(struct_comma_semi "neg/struct_comma_semi.poc")
add_poco_neg_test(struct_dot_on_ptr "neg/struct_dot_on_ptr.poc")
add_poco_neg_test(struct_in_for_loop "neg/struct_in_for_loop.poc")
add_poco_neg_test(struct_in_parens "neg/struct_in_parens.poc")
add_poco_neg_test(struct_keyword_dup "neg/struct_keyword_dup.poc")
add_poco_neg_test(struct_keyword_name "neg/struct_keyword_name.poc")
add_poco_neg_test(struct_member_keyword "neg/struct_member_keyword.poc")
add_poco_neg_test(struct_missing_brace "neg/struct_missing_brace.poc")
add_poco_neg_test(struct_missing_keyword "neg/struct_missing_keyword.poc")
add_poco_neg_test(struct_missing_semi "neg/struct_missing_semi.poc")
add_poco_neg_test(token_empty_file "neg/token_empty_file.poc")
add_poco_neg_test(token_main_only "neg/token_main_only.poc")
add_poco_neg_test(token_no_main "neg/token_no_main.poc")
add_poco_neg_test(token_only_comment "neg/token_only_comment.poc")
add_poco_neg_test(type_funcptr_mismatch "neg/type_funcptr_mismatch.poc")
add_poco_neg_test(type_funcptr_params "neg/type_funcptr_params.poc")
add_poco_neg_test(undefined_var "neg/undefined_var.poc")
add_poco_neg_test(void_return_value "neg/void_return_value.poc")

# Library loading error tests - install test files first
INSTALL(FILES 
    ${CMAKE_CURRENT_SOURCE_DIR}/neg/nlib01.poc
    ${CMAKE_CURRENT_SOURCE_DIR}/neg/nlib02.poc
    ${CMAKE_CURRENT_SOURCE_DIR}/neg/nlib03.poc
    ${CMAKE_CURRENT_SOURCE_DIR}/neg/nlib04.poc
    ${CMAKE_CURRENT_SOURCE_DIR}/neg/nlib05.poc
    DESTINATION ${CMAKE_INSTALL_PREFIX}/tests/
)

# Define tests using installed files
add_test(
    NAME "poco_neg_lib01_not_found"
    COMMAND ${CMAKE_INSTALL_PREFIX}/poco ${CMAKE_INSTALL_PREFIX}/tests/nlib01.poc
    WORKING_DIRECTORY "${CMAKE_INSTALL_PREFIX}"
)
set_tests_properties("poco_neg_lib01_not_found" PROPERTIES
    WILL_FAIL TRUE
    TIMEOUT 10
)

add_test(
    NAME "poco_neg_lib02_no_entry"
    COMMAND ${CMAKE_INSTALL_PREFIX}/poco ${CMAKE_INSTALL_PREFIX}/tests/nlib02.poc
    WORKING_DIRECTORY "${CMAKE_INSTALL_PREFIX}"
)
set_tests_properties("poco_neg_lib02_no_entry" PROPERTIES
    WILL_FAIL TRUE
    TIMEOUT 10
    DEPENDS test_noentry
)

add_test(
    NAME "poco_neg_lib03_null_ret"
    COMMAND ${CMAKE_INSTALL_PREFIX}/poco ${CMAKE_INSTALL_PREFIX}/tests/nlib03.poc
    WORKING_DIRECTORY "${CMAKE_INSTALL_PREFIX}"
)
set_tests_properties("poco_neg_lib03_null_ret" PROPERTIES
    WILL_FAIL TRUE
    TIMEOUT 10
    DEPENDS test_nullret
)

add_test(
    NAME "poco_neg_lib04_bad_ver"
    COMMAND ${CMAKE_INSTALL_PREFIX}/poco ${CMAKE_INSTALL_PREFIX}/tests/nlib04.poc
    WORKING_DIRECTORY "${CMAKE_INSTALL_PREFIX}"
)
set_tests_properties("poco_neg_lib04_bad_ver" PROPERTIES
    WILL_FAIL TRUE
    TIMEOUT 10
    DEPENDS test_badver
)

add_test(
    NAME "poco_neg_lib05_empty"
    COMMAND ${CMAKE_INSTALL_PREFIX}/poco ${CMAKE_INSTALL_PREFIX}/tests/nlib05.poc
    WORKING_DIRECTORY "${CMAKE_INSTALL_PREFIX}"
)
set_tests_properties("poco_neg_lib05_empty" PROPERTIES
    WILL_FAIL TRUE
    TIMEOUT 10
    DEPENDS test_empty
)


###############################################################################
# General Tests - Various poco scripts in the root test directory
###############################################################################

add_poco_test(hello "hello.poc")
add_poco_test(array "array.poc")
add_poco_test(ffi_01 "po_ffi_01.poc")

# Note: The hello library test is now auto-generated by the add_poe_library() 
# helper function in poco/poekit/hello/CMakeLists.txt. It creates a test named
# "poco_hello" that loads and runs hello.poc from the install directory.

# Add more general tests as needed:
# add_poco_test(atof "ATOF.POC")
# add_poco_test(comma "COMMA.POC")
# add_poco_test(float "FLOAT.POC")
# add_poco_test(forever "FOREVER.POC")
# add_poco_test(if "IF.POC")
# add_poco_test(short "SHORT.POC")
# add_poco_test(struct "STRUCT.POC")
# etc...

