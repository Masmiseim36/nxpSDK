# Add set(CONFIG_USE_component_els_pkc_examples_els true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

      target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
          ${CMAKE_CURRENT_LIST_DIR}/examples/mcuxClEls/mcuxClEls_Cipher_Aes128_Cbc_Encrypt_example.c
          ${CMAKE_CURRENT_LIST_DIR}/examples/mcuxClEls/mcuxClEls_Cipher_Aes128_Ecb_Encrypt_example.c
          ${CMAKE_CURRENT_LIST_DIR}/examples/mcuxClEls/mcuxClEls_Common_Get_Info_example.c
          ${CMAKE_CURRENT_LIST_DIR}/examples/mcuxClEls/mcuxClEls_Ecc_Keygen_Sign_Verify_example.c
          ${CMAKE_CURRENT_LIST_DIR}/examples/mcuxClEls/mcuxClEls_Hash_HW_Security_Counter_example.c
          ${CMAKE_CURRENT_LIST_DIR}/examples/mcuxClEls/mcuxClEls_Hash_Sha224_One_Block_example.c
          ${CMAKE_CURRENT_LIST_DIR}/examples/mcuxClEls/mcuxClEls_Hash_Sha256_One_Block_example.c
          ${CMAKE_CURRENT_LIST_DIR}/examples/mcuxClEls/mcuxClEls_Hash_Sha384_One_Block_example.c
          ${CMAKE_CURRENT_LIST_DIR}/examples/mcuxClEls/mcuxClEls_Hash_Sha512_One_Block_example.c
          ${CMAKE_CURRENT_LIST_DIR}/examples/mcuxClEls/mcuxClEls_Rng_Prng_Get_Random_example.c
          ${CMAKE_CURRENT_LIST_DIR}/examples/mcuxClEls/mcuxClEls_Tls_Master_Key_Session_Keys_example.c
          ${CMAKE_CURRENT_LIST_DIR}/examples/mcuxClRandomModes/mcuxClRandomModes_ELS_example.c
        )

  
      target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
          ${CMAKE_CURRENT_LIST_DIR}/.
          ${CMAKE_CURRENT_LIST_DIR}/examples/mcuxCsslFlowProtection/inc
          ${CMAKE_CURRENT_LIST_DIR}/examples/mcuxCsslMemory/inc
          ${CMAKE_CURRENT_LIST_DIR}/src/comps/mcuxClExample/inc
          ${CMAKE_CURRENT_LIST_DIR}/src/comps/mcuxClBuffer/inc
        )

  
