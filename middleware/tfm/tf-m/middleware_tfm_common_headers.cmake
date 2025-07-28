# Add set(CONFIG_USE_middleware_tfm_common_headers true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

      target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
          ${CMAKE_CURRENT_LIST_DIR}/../../mbedtls3x/include
          ${CMAKE_CURRENT_LIST_DIR}/platform/include
          ${CMAKE_CURRENT_LIST_DIR}/interface/include
          ${CMAKE_CURRENT_LIST_DIR}/interface/include/psa
          ${CMAKE_CURRENT_LIST_DIR}/interface/include/crypto_keys
          ${CMAKE_CURRENT_LIST_DIR}/interface/include/psa_manifest
        )

  
