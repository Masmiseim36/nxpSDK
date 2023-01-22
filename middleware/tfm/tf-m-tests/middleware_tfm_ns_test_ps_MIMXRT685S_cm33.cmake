include_guard()
message("middleware_tfm_ns_test_ps component is included.")

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/test/secure_fw/suites/ps/non_secure/ns_test_helpers.c
    ${CMAKE_CURRENT_LIST_DIR}/test/secure_fw/suites/ps/non_secure/psa_ps_ns_interface_testsuite.c
)


target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/test/secure_fw/suites/ps/non_secure
)


