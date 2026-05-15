# Add set(CONFIG_USE_component_psa_crypto_driver_sgi_transparent true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

      target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
          ${CMAKE_CURRENT_LIST_DIR}/sgi/src/transparent/mcux_psa_sgi_aead.c
          ${CMAKE_CURRENT_LIST_DIR}/sgi/src/transparent/mcux_psa_sgi_cipher.c
          ${CMAKE_CURRENT_LIST_DIR}/sgi/src/transparent/mcux_psa_sgi_hash.c
          ${CMAKE_CURRENT_LIST_DIR}/sgi/src/transparent/mcux_psa_sgi_init.c
          ${CMAKE_CURRENT_LIST_DIR}/sgi/src/transparent/mcux_psa_sgi_mac.c
        )

  
      target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
          ${CMAKE_CURRENT_LIST_DIR}/sgi/include/transparent
        )

  
