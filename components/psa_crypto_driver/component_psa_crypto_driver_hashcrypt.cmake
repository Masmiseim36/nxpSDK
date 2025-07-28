# Add set(CONFIG_USE_component_psa_crypto_driver_hashcrypt true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

      target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
          ${CMAKE_CURRENT_LIST_DIR}/hashcrypt/src/mcux_psa_hashcrypt_common_cipher.c
          ${CMAKE_CURRENT_LIST_DIR}/hashcrypt/src/mcux_psa_hashcrypt_common_init.c
          ${CMAKE_CURRENT_LIST_DIR}/hashcrypt/src/mcux_psa_hashcrypt_entropy.c
          ${CMAKE_CURRENT_LIST_DIR}/hashcrypt/src/mcux_psa_hashcrypt_hash.c
          ${CMAKE_CURRENT_LIST_DIR}/hashcrypt/src/mcux_psa_hashcrypt_init.c
        )

  
      target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
          ${CMAKE_CURRENT_LIST_DIR}/hashcrypt
          ${CMAKE_CURRENT_LIST_DIR}/hashcrypt/include
        )

    if(CONFIG_USE_COMPONENT_CONFIGURATION)
  message("===>Import configuration from ${CMAKE_CURRENT_LIST_FILE}")

      target_compile_definitions(${MCUX_SDK_PROJECT_NAME} PUBLIC
                  -DPSA_CRYPTO_DRIVER_HASHCRYPT
              )
  
  
  endif()

