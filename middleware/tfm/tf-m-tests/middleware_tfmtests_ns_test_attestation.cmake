# Add set(CONFIG_USE_middleware_tfmtests_ns_test_attestation true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}/test/secure_fw/suites/attestation/ext/qcbor_util/qcbor_util.c
  ${CMAKE_CURRENT_LIST_DIR}/test/secure_fw/suites/attestation/attest_token_decode_common.c
  ${CMAKE_CURRENT_LIST_DIR}/test/secure_fw/suites/attestation/attest_token_test.c
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/test/secure_fw/suites/attestation/ext/qcbor_util
  ${CMAKE_CURRENT_LIST_DIR}/test/secure_fw/suites/attestation/non_secure
  ${CMAKE_CURRENT_LIST_DIR}/test/secure_fw/suites/attestation
)

if(CONFIG_USE_COMPONENT_CONFIGURATION)
  message("===>Import configuration from ${CMAKE_CURRENT_LIST_FILE}")

  target_compile_definitions(${MCUX_SDK_PROJECT_NAME} PUBLIC
    -DTEST_NS_ATTESTATION
  )

endif()

