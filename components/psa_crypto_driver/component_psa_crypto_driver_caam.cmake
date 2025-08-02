# Add set(CONFIG_USE_component_psa_crypto_driver_caam true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

      target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
          ${CMAKE_CURRENT_LIST_DIR}/caam/src/mcux_psa_caam_common_aead.c
          ${CMAKE_CURRENT_LIST_DIR}/caam/src/mcux_psa_caam_common_asymmetric_encryption.c
          ${CMAKE_CURRENT_LIST_DIR}/caam/src/mcux_psa_caam_common_asymmetric_signature.c
          ${CMAKE_CURRENT_LIST_DIR}/caam/src/mcux_psa_caam_common_cipher.c
          ${CMAKE_CURRENT_LIST_DIR}/caam/src/mcux_psa_caam_common_init.c
          ${CMAKE_CURRENT_LIST_DIR}/caam/src/mcux_psa_caam_common_key_generation.c
          ${CMAKE_CURRENT_LIST_DIR}/caam/src/mcux_psa_caam_common_mac.c
          ${CMAKE_CURRENT_LIST_DIR}/caam/src/mcux_psa_caam_entropy.c
          ${CMAKE_CURRENT_LIST_DIR}/caam/src/mcux_psa_caam_hash.c
          ${CMAKE_CURRENT_LIST_DIR}/caam/src/mcux_psa_caam_init.c
          ${CMAKE_CURRENT_LIST_DIR}/caam/src/mcux_psa_caam_utils.c
          ${CMAKE_CURRENT_LIST_DIR}/caam/common/src/mcux_psa_common_key_management.c
          ${CMAKE_CURRENT_LIST_DIR}/caam/common/src/mcux_psa_mbedtls_origin.c
        )

  
      target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
          ${CMAKE_CURRENT_LIST_DIR}/caam
          ${CMAKE_CURRENT_LIST_DIR}/caam/include
          ${CMAKE_CURRENT_LIST_DIR}/caam/common/include
        )

    if(CONFIG_USE_COMPONENT_CONFIGURATION)
  message("===>Import configuration from ${CMAKE_CURRENT_LIST_FILE}")

      target_compile_definitions(${MCUX_SDK_PROJECT_NAME} PUBLIC
                  -DPSA_CRYPTO_DRIVER_CAAM
              )
  
  
  endif()

