# Add set(CONFIG_USE_component_els_pkc_rsa_oaep true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

      target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
          ${CMAKE_CURRENT_LIST_DIR}/src/comps/mcuxClRsa/src/mcuxClRsa_KeyTypes.c
          ${CMAKE_CURRENT_LIST_DIR}/src/comps/mcuxClRsa/src/mcuxClRsa_ModeConstructors.c
          ${CMAKE_CURRENT_LIST_DIR}/src/comps/mcuxClRsa/src/mcuxClRsa_OaepDecode.c
          ${CMAKE_CURRENT_LIST_DIR}/src/comps/mcuxClRsa/src/mcuxClRsa_OaepEncode.c
          ${CMAKE_CURRENT_LIST_DIR}/src/comps/mcuxClRsa/src/mcuxClRsa_pkcs1v15Decode_decrypt.c
          ${CMAKE_CURRENT_LIST_DIR}/src/comps/mcuxClRsa/src/mcuxClRsa_Pkcs1v15Encode_encrypt.c
          ${CMAKE_CURRENT_LIST_DIR}/src/comps/mcuxClRsa/src/mcuxClRsa_Util_Decrypt.c
          ${CMAKE_CURRENT_LIST_DIR}/src/comps/mcuxClRsa/src/mcuxClRsa_Util_Encrypt.c
        )

  
      target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
          ${CMAKE_CURRENT_LIST_DIR}/.
          ${CMAKE_CURRENT_LIST_DIR}/src/comps/mcuxClRsa/inc
          ${CMAKE_CURRENT_LIST_DIR}/src/comps/mcuxClRsa/inc/internal
        )

  
