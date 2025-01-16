# Add set(CONFIG_USE_component_els_pkc_cipher_modes true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

      target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
          ${CMAKE_CURRENT_LIST_DIR}/src/comps/mcuxClCipherModes/src/mcuxClCipherModes_Common_Helper.c
          ${CMAKE_CURRENT_LIST_DIR}/src/comps/mcuxClCipherModes/src/mcuxClCipherModes_Crypt_Els_Modes.c
          ${CMAKE_CURRENT_LIST_DIR}/src/comps/mcuxClCipherModes/src/mcuxClCipherModes_Els_Aes.c
          ${CMAKE_CURRENT_LIST_DIR}/src/comps/mcuxClCipherModes/src/mcuxClCipherModes_Els_AesEngine.c
          ${CMAKE_CURRENT_LIST_DIR}/src/comps/mcuxClCipherModes/src/mcuxClCipherModes_Els_Aes_Internal.c
        )

  
      target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
          ${CMAKE_CURRENT_LIST_DIR}/.
          ${CMAKE_CURRENT_LIST_DIR}/src/comps/mcuxClCipherModes/inc
          ${CMAKE_CURRENT_LIST_DIR}/src/comps/mcuxClCipherModes/inc/internal
        )

  
