# Add set(CONFIG_USE_middleware_tfm_ns_test_profile_medium true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

if(CONFIG_USE_middleware_tfm_ns_test_its AND CONFIG_USE_middleware_tfm_ns_test_attestation_asymmetric AND CONFIG_USE_middleware_tfm_ns_test_crypto)

if(CONFIG_USE_COMPONENT_CONFIGURATION)
  message("===>Import configuration from ${CMAKE_CURRENT_LIST_FILE}")

  target_compile_definitions(${MCUX_SDK_PROJECT_NAME} PUBLIC
    -DTFM_CRYPTO_TEST_ALG_CCM
    -DTFM_CRYPTO_TEST_ECDH
  )

endif()

else()

message(SEND_ERROR "middleware_tfm_ns_test_profile_medium dependency does not meet, please check ${CMAKE_CURRENT_LIST_FILE}.")

endif()
