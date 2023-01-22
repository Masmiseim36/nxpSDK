include_guard()
message("middleware_tfmtests_ns_test_attestation component is included.")

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/test/secure_fw/suites/attestation/ext/qcbor_util/qcbor_util.c
    ${CMAKE_CURRENT_LIST_DIR}/test/secure_fw/suites/attestation/non_secure/attest_asymmetric_ns_interface_testsuite.c
    ${CMAKE_CURRENT_LIST_DIR}/test/secure_fw/suites/attestation/attest_token_decode_asymmetric.c
    ${CMAKE_CURRENT_LIST_DIR}/test/secure_fw/suites/attestation/attest_token_decode_common.c
    ${CMAKE_CURRENT_LIST_DIR}/test/secure_fw/suites/attestation/attest_token_test.c
)


target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/test/secure_fw/suites/attestation/ext/qcbor_util
    ${CMAKE_CURRENT_LIST_DIR}/test/secure_fw/suites/attestation/non_secure
    ${CMAKE_CURRENT_LIST_DIR}/test/secure_fw/suites/attestation
)


