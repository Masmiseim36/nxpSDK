# Add set(CONFIG_USE_middleware_tfm_s_test_attestation true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

      target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
          ${CMAKE_CURRENT_LIST_DIR}/tests_reg/test/secure_fw/suites/attestation/ext/qcbor_util/qcbor_util.c
          ${CMAKE_CURRENT_LIST_DIR}/tests_reg/test/secure_fw/suites/attestation/attest_token_decode_common.c
          ${CMAKE_CURRENT_LIST_DIR}/tests_reg/test/secure_fw/suites/attestation/attest_token_test.c
          ${CMAKE_CURRENT_LIST_DIR}/lib/log/tfm_log_raw.c
        )

  
      target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
          ${CMAKE_CURRENT_LIST_DIR}/tests_reg/test/secure_fw/suites/attestation/ext/qcbor_util
          ${CMAKE_CURRENT_LIST_DIR}/tests_reg/test/secure_fw/suites/attestation/secure
          ${CMAKE_CURRENT_LIST_DIR}/tests_reg/test/secure_fw/suites/attestation
          ${CMAKE_CURRENT_LIST_DIR}/tests_reg/test/framework
          ${CMAKE_CURRENT_LIST_DIR}/lib/log
          ${CMAKE_CURRENT_LIST_DIR}/../qcbor/inc
          ${CMAKE_CURRENT_LIST_DIR}/../tf-m/secure_fw/spm/include/boot
        )

    if(CONFIG_USE_COMPONENT_CONFIGURATION)
  message("===>Import configuration from ${CMAKE_CURRENT_LIST_FILE}")

      target_compile_definitions(${MCUX_SDK_PROJECT_NAME} PUBLIC
                  -DTFM_PARTITION_INITIAL_ATTESTATION
                        -DT_COSE_USE_PSA_CRYPTO
              )
  
  
  endif()

