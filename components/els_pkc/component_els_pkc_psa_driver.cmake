# Add set(CONFIG_USE_component_els_pkc_psa_driver true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

      target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
          ${CMAKE_CURRENT_LIST_DIR}/src/comps/mcuxClPsaDriver/src/mcuxClPsaDriver_Aead.c
          ${CMAKE_CURRENT_LIST_DIR}/src/comps/mcuxClPsaDriver/src/mcuxClPsaDriver_Cipher.c
          ${CMAKE_CURRENT_LIST_DIR}/src/comps/mcuxClPsaDriver/src/mcuxClPsaDriver_DER_functions.c
          ${CMAKE_CURRENT_LIST_DIR}/src/comps/mcuxClPsaDriver/src/mcuxClPsaDriver_export_public_key.c
          ${CMAKE_CURRENT_LIST_DIR}/src/comps/mcuxClPsaDriver/src/mcuxClPsaDriver_generate_ecp_key.c
          ${CMAKE_CURRENT_LIST_DIR}/src/comps/mcuxClPsaDriver/src/mcuxClPsaDriver_Hash.c
          ${CMAKE_CURRENT_LIST_DIR}/src/comps/mcuxClPsaDriver/src/mcuxClPsaDriver_Key.c
          ${CMAKE_CURRENT_LIST_DIR}/src/comps/mcuxClPsaDriver/src/mcuxClPsaDriver_Mac.c
          ${CMAKE_CURRENT_LIST_DIR}/src/comps/mcuxClPsaDriver/src/mcuxClPsaDriver_Rsa.c
          ${CMAKE_CURRENT_LIST_DIR}/src/comps/mcuxClPsaDriver/src/mcuxClPsaDriver_Sign.c
          ${CMAKE_CURRENT_LIST_DIR}/src/comps/mcuxClPsaDriver/src/mcuxClPsaDriver_UpdateKeyStatus.c
          ${CMAKE_CURRENT_LIST_DIR}/src/comps/mcuxClPsaDriver/src/mcuxClPsaDriver_Verify.c
        )

  
      target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
          ${CMAKE_CURRENT_LIST_DIR}/.
          ${CMAKE_CURRENT_LIST_DIR}/src/comps/mcuxClPsaDriver/inc
          ${CMAKE_CURRENT_LIST_DIR}/src/comps/mcuxClPsaDriver/inc/internal
        )

  
