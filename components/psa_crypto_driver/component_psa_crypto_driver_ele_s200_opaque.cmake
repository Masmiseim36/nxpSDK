# Add set(CONFIG_USE_component_psa_crypto_driver_ele_s200_opaque true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

      target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
          ${CMAKE_CURRENT_LIST_DIR}/ele_s200/src/opaque/mcux_psa_s2xx_opaque_key_generation.c
          ${CMAKE_CURRENT_LIST_DIR}/ele_s200/src/opaque/mcux_psa_s2xx_opaque_cipher.c
          ${CMAKE_CURRENT_LIST_DIR}/ele_s200/src/opaque/mcux_psa_s2xx_opaque_aead.c
          ${CMAKE_CURRENT_LIST_DIR}/ele_s200/src/opaque/mcux_psa_s2xx_opaque_asymmetric_signature.c
          ${CMAKE_CURRENT_LIST_DIR}/ele_s200/src/opaque/mcux_psa_s2xx_opaque_mac.c
        )

  
      target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
          ${CMAKE_CURRENT_LIST_DIR}/ele_s200/include/opaque
        )

  
