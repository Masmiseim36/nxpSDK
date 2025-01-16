# Add set(CONFIG_USE_component_els_pkc_oscca_sm2 true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

      target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
          ${CMAKE_CURRENT_LIST_DIR}/src/comps/mcuxClOsccaSm2/src/mcuxClOsccaSm2_Cipher_Crypt.c
          ${CMAKE_CURRENT_LIST_DIR}/src/comps/mcuxClOsccaSm2/src/mcuxClOsccaSm2_ComputePrehash.c
          ${CMAKE_CURRENT_LIST_DIR}/src/comps/mcuxClOsccaSm2/src/mcuxClOsccaSm2_Constants.c
          ${CMAKE_CURRENT_LIST_DIR}/src/comps/mcuxClOsccaSm2/src/mcuxClOsccaSm2_CryptoUtils.c
          ${CMAKE_CURRENT_LIST_DIR}/src/comps/mcuxClOsccaSm2/src/mcuxClOsccaSm2_Decrypt.c
          ${CMAKE_CURRENT_LIST_DIR}/src/comps/mcuxClOsccaSm2/src/mcuxClOsccaSm2_Ecc.c
          ${CMAKE_CURRENT_LIST_DIR}/src/comps/mcuxClOsccaSm2/src/mcuxClOsccaSm2_EccUtils.c
          ${CMAKE_CURRENT_LIST_DIR}/src/comps/mcuxClOsccaSm2/src/mcuxClOsccaSm2_EncDec_Internal.c
          ${CMAKE_CURRENT_LIST_DIR}/src/comps/mcuxClOsccaSm2/src/mcuxClOsccaSm2_Encrypt.c
          ${CMAKE_CURRENT_LIST_DIR}/src/comps/mcuxClOsccaSm2/src/mcuxClOsccaSm2_FUP.c
          ${CMAKE_CURRENT_LIST_DIR}/src/comps/mcuxClOsccaSm2/src/mcuxClOsccaSm2_GenerateKeyPair.c
          ${CMAKE_CURRENT_LIST_DIR}/src/comps/mcuxClOsccaSm2/src/mcuxClOsccaSm2_Helper.c
          ${CMAKE_CURRENT_LIST_DIR}/src/comps/mcuxClOsccaSm2/src/mcuxClOsccaSm2_InvertPrivateKey.c
          ${CMAKE_CURRENT_LIST_DIR}/src/comps/mcuxClOsccaSm2/src/mcuxClOsccaSm2_Keyagreement.c
          ${CMAKE_CURRENT_LIST_DIR}/src/comps/mcuxClOsccaSm2/src/mcuxClOsccaSm2_Keyagreement_SelfTest.c
          ${CMAKE_CURRENT_LIST_DIR}/src/comps/mcuxClOsccaSm2/src/mcuxClOsccaSm2_KeyExchange.c
          ${CMAKE_CURRENT_LIST_DIR}/src/comps/mcuxClOsccaSm2/src/mcuxClOsccaSm2_KeyTypes.c
          ${CMAKE_CURRENT_LIST_DIR}/src/comps/mcuxClOsccaSm2/src/mcuxClOsccaSm2_SelfTest.c
          ${CMAKE_CURRENT_LIST_DIR}/src/comps/mcuxClOsccaSm2/src/mcuxClOsccaSm2_Sign.c
          ${CMAKE_CURRENT_LIST_DIR}/src/comps/mcuxClOsccaSm2/src/mcuxClOsccaSm2_Signature_Internal.c
          ${CMAKE_CURRENT_LIST_DIR}/src/comps/mcuxClOsccaSm2/src/mcuxClOsccaSm2_Signature_PreHash.c
          ${CMAKE_CURRENT_LIST_DIR}/src/comps/mcuxClOsccaSm2/src/mcuxClOsccaSm2_Signature_SelfTest.c
          ${CMAKE_CURRENT_LIST_DIR}/src/comps/mcuxClOsccaSm2/src/mcuxClOsccaSm2_Verify.c
        )

  
      target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
          ${CMAKE_CURRENT_LIST_DIR}/.
          ${CMAKE_CURRENT_LIST_DIR}/src/comps/mcuxClOsccaSm2/inc
          ${CMAKE_CURRENT_LIST_DIR}/src/comps/mcuxClOsccaSm2/inc/internal
        )

  
