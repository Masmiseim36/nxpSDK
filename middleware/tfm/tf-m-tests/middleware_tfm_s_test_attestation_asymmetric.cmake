# Add set(CONFIG_USE_middleware_tfm_s_test_attestation_asymmetric true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

      target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
          ${CMAKE_CURRENT_LIST_DIR}/tests_reg/test/secure_fw/suites/attestation/secure/attest_asymmetric_s_interface_testsuite.c
          ${CMAKE_CURRENT_LIST_DIR}/tests_reg/test/secure_fw/suites/attestation/attest_token_decode_asymmetric.c
          ${CMAKE_CURRENT_LIST_DIR}/lib/log/tfm_log_raw.c
        )

  
      target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
          ${CMAKE_CURRENT_LIST_DIR}/lib/log
          ${CMAKE_CURRENT_LIST_DIR}/../tf-m/secure_fw/spm/include/boot
          ${CMAKE_CURRENT_LIST_DIR}/../tf-m/secure_fw/partitions/initial_attestation
        )

    if(CONFIG_USE_COMPONENT_CONFIGURATION)
  message("===>Import configuration from ${CMAKE_CURRENT_LIST_FILE}")

      target_compile_definitions(${MCUX_SDK_PROJECT_NAME} PUBLIC
                  -DATTEST_KEY_BITS=256
                        -DT_COSE_DISABLE_MAC0
                        -DT_COSE_DISABLE_ES384
                        -DT_COSE_DISABLE_ES512
              )
  
  
  endif()

