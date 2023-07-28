# Add set(CONFIG_USE_middleware_tfm_s_test_attestation_symmetric true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

if(CONFIG_USE_middleware_tfm_s_test_attestation AND CONFIG_USE_middleware_tfm_t_cose_mac0_verify)

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}/test/secure_fw/suites/attestation/secure/attest_symmetric_s_interface_testsuite.c
  ${CMAKE_CURRENT_LIST_DIR}/test/secure_fw/suites/attestation/attest_token_decode_symmetric.c
)

else()

message(SEND_ERROR "middleware_tfm_s_test_attestation_symmetric dependency does not meet, please check ${CMAKE_CURRENT_LIST_FILE}.")

endif()
