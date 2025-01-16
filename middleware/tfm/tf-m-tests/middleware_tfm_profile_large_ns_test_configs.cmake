# Add set(CONFIG_USE_middleware_tfm_profile_large_ns_test_configs true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

  if(CONFIG_USE_COMPONENT_CONFIGURATION)
  message("===>Import configuration from ${CMAKE_CURRENT_LIST_FILE}")

      target_compile_definitions(${MCUX_SDK_PROJECT_NAME} PUBLIC
                  -DTFM_CRYPTO_TEST_ALG_CBC
                        -DTFM_CRYPTO_TEST_ALG_CCM
                        -DTFM_CRYPTO_TEST_ALG_GCM
                        -DTFM_CRYPTO_TEST_ALG_SHA_512
                        -DTFM_CRYPTO_TEST_HKDF
                        -DTFM_CRYPTO_TEST_ECDH
                        -DTFM_CRYPTO_TEST_UNSUPPORTED_ALG
              )
  
  
  endif()

