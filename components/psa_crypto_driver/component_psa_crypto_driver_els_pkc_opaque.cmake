# Add set(CONFIG_USE_component_psa_crypto_driver_els_pkc_opaque true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

      target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
          ${CMAKE_CURRENT_LIST_DIR}/els_pkc/src/opaque/mcux_psa_els_pkc_opaque_aead.c
          ${CMAKE_CURRENT_LIST_DIR}/els_pkc/src/opaque/mcux_psa_els_pkc_opaque_asymmetric_signature.c
          ${CMAKE_CURRENT_LIST_DIR}/els_pkc/src/opaque/mcux_psa_els_pkc_opaque_cipher.c
          ${CMAKE_CURRENT_LIST_DIR}/els_pkc/src/opaque/mcux_psa_els_pkc_opaque_init.c
          ${CMAKE_CURRENT_LIST_DIR}/els_pkc/src/opaque/mcux_psa_els_pkc_opaque_key_generation.c
          ${CMAKE_CURRENT_LIST_DIR}/els_pkc/src/opaque/mcux_psa_els_pkc_opaque_mac.c
        )

  
      target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
          ${CMAKE_CURRENT_LIST_DIR}/els_pkc/include/opaque
        )

  
