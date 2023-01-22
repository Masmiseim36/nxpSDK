include_guard()
message("middleware_tfm_ns_test_crypto component is included.")

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/test/secure_fw/suites/crypto/crypto_tests_common.c
    ${CMAKE_CURRENT_LIST_DIR}/test/secure_fw/suites/crypto/non_secure/crypto_ns_interface_testsuite.c
)


target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/test/secure_fw/suites/crypto
    ${CMAKE_CURRENT_LIST_DIR}/test/secure_fw/suites/crypto/non_secure
)


