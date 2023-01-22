include_guard()
message("middleware_tfm_s_test_attestation component is included.")

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/test/secure_fw/suites/attestation/ext/qcbor_util/qcbor_util.c
    ${CMAKE_CURRENT_LIST_DIR}/test/secure_fw/suites/attestation/secure/attest_asymmetric_s_interface_testsuite.c
    ${CMAKE_CURRENT_LIST_DIR}/test/secure_fw/suites/attestation/attest_token_decode_asymmetric.c
    ${CMAKE_CURRENT_LIST_DIR}/test/secure_fw/suites/attestation/attest_token_decode_common.c
    ${CMAKE_CURRENT_LIST_DIR}/test/secure_fw/suites/attestation/attest_token_test.c
)


target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/test/secure_fw/suites/attestation/ext/qcbor_util
    ${CMAKE_CURRENT_LIST_DIR}/test/secure_fw/suites/attestation/secure
    ${CMAKE_CURRENT_LIST_DIR}/test/secure_fw/suites/attestation
)


include(middleware_tfm_t_cose_sign1_verify_MIMXRT595S_cm33)

