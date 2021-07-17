include_guard(GLOBAL)
message("middleware_tfm_s_test_crypto component is included.")

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/test/suites/crypto/crypto_tests_common.c
    ${CMAKE_CURRENT_LIST_DIR}/test/suites/crypto/secure/crypto_sec_interface_testsuite.c
)


target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/test/suites/crypto
    ${CMAKE_CURRENT_LIST_DIR}/test/suites/crypto/secure
)


