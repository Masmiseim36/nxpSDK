include_guard(GLOBAL)
message("middleware_tfm_ns_test_crypto component is included.")

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/test/suites/crypto/crypto_tests_common.c
    ${CMAKE_CURRENT_LIST_DIR}/test/suites/crypto/non_secure/crypto_ns_interface_testsuite.c
)


target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/test/suites/crypto
    ${CMAKE_CURRENT_LIST_DIR}/test/suites/crypto/non_secure
)


