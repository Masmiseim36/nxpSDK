# Add set(CONFIG_USE_component_els_pkc_key_derivation true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

      target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
          ${CMAKE_CURRENT_LIST_DIR}/src/comps/mcuxClKey/src/mcuxClKey_agreement.c
          ${CMAKE_CURRENT_LIST_DIR}/src/comps/mcuxClKey/src/mcuxClKey_agreement_selftest.c
          ${CMAKE_CURRENT_LIST_DIR}/src/comps/mcuxClKey/src/mcuxClKey_Derivation.c
          ${CMAKE_CURRENT_LIST_DIR}/src/comps/mcuxClKey/src/mcuxClKey_Derivation_HKDF.c
          ${CMAKE_CURRENT_LIST_DIR}/src/comps/mcuxClKey/src/mcuxClKey_Derivation_NIST_SP800_108.c
          ${CMAKE_CURRENT_LIST_DIR}/src/comps/mcuxClKey/src/mcuxClKey_Derivation_NIST_SP800_56C.c
          ${CMAKE_CURRENT_LIST_DIR}/src/comps/mcuxClKey/src/mcuxClKey_Derivation_PBKDF2.c
          ${CMAKE_CURRENT_LIST_DIR}/src/comps/mcuxClKey/src/mcuxClKey_generate_keypair.c
        )

  
      target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
          ${CMAKE_CURRENT_LIST_DIR}/.
          ${CMAKE_CURRENT_LIST_DIR}/src/comps/mcuxClKey/inc
          ${CMAKE_CURRENT_LIST_DIR}/src/comps/mcuxClKey/inc/internal
        )

  
