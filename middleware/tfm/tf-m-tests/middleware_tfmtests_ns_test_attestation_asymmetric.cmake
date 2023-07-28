# Add set(CONFIG_USE_middleware_tfmtests_ns_test_attestation_asymmetric true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

if(CONFIG_USE_middleware_tfmtests_ns_test_attestation)

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}/test/secure_fw/suites/attestation/non_secure/attest_asymmetric_ns_interface_testsuite.c
  ${CMAKE_CURRENT_LIST_DIR}/test/secure_fw/suites/attestation/attest_token_decode_asymmetric.c
)

else()

message(SEND_ERROR "middleware_tfmtests_ns_test_attestation_asymmetric dependency does not meet, please check ${CMAKE_CURRENT_LIST_FILE}.")

endif()
