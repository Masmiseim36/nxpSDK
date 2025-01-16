# Add set(CONFIG_USE_component_els_pkc_examples_rsa true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

      target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
          ${CMAKE_CURRENT_LIST_DIR}/examples/mcuxClRsa/mcuxClRsa_sign_NoEncode_example.c
          ${CMAKE_CURRENT_LIST_DIR}/examples/mcuxClRsa/mcuxClRsa_sign_pss_sha2_256_example.c
          ${CMAKE_CURRENT_LIST_DIR}/examples/mcuxClRsa/mcuxClRsa_verify_NoVerify_example.c
          ${CMAKE_CURRENT_LIST_DIR}/examples/mcuxClRsa/mcuxClRsa_verify_pssverify_sha2_256_example.c
        )

  
      target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
          ${CMAKE_CURRENT_LIST_DIR}/.
          ${CMAKE_CURRENT_LIST_DIR}/examples/mcuxCsslFlowProtection/inc
          ${CMAKE_CURRENT_LIST_DIR}/examples/mcuxCsslMemory/inc
          ${CMAKE_CURRENT_LIST_DIR}/src/comps/mcuxClExample/inc
          ${CMAKE_CURRENT_LIST_DIR}/src/comps/mcuxClBuffer/inc
        )

  
