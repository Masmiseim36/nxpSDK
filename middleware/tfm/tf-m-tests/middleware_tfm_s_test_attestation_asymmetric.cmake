# Add set(CONFIG_USE_middleware_tfm_s_test_attestation_asymmetric true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

      target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
          ${CMAKE_CURRENT_LIST_DIR}/test/secure_fw/suites/attestation/secure/attest_asymmetric_s_interface_testsuite.c
          ${CMAKE_CURRENT_LIST_DIR}/test/secure_fw/suites/attestation/attest_token_decode_asymmetric.c
        )

  

