# Add set(CONFIG_USE_middleware_tfm_ns_test_tcose true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

if(CONFIG_USE_middleware_tfmtests_ns_test_tcose)

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}/lib/ext/t_cose/test/t_cose_make_test_messages.c
  ${CMAKE_CURRENT_LIST_DIR}/lib/ext/t_cose/test/t_cose_make_psa_test_key.c
  ${CMAKE_CURRENT_LIST_DIR}/lib/ext/t_cose/test/t_cose_sign_verify_test.c
  ${CMAKE_CURRENT_LIST_DIR}/lib/ext/t_cose/test/t_cose_test.c
  ${CMAKE_CURRENT_LIST_DIR}/lib/ext/t_cose/test/run_tests.c
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/lib/ext/t_cose/test
)

if(CONFIG_USE_COMPONENT_CONFIGURATION)
  message("===>Import configuration from ${CMAKE_CURRENT_LIST_FILE}")

  target_compile_definitions(${MCUX_SDK_PROJECT_NAME} PUBLIC
    -DTEST_NS_T_COSE
    -DT_COSE_DISABLE_ES384
    -DT_COSE_DISABLE_ES512
    -DT_COSE_DISABLE_SIGN_VERIFY_TESTS
    -DT_COSE_USE_PSA_CRYPTO_FROM_TFM
  )

endif()

else()

message(SEND_ERROR "middleware_tfm_ns_test_tcose dependency does not meet, please check ${CMAKE_CURRENT_LIST_FILE}.")

endif()
