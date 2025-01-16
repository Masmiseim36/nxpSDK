# Add set(CONFIG_USE_component_els_pkc_examples_random_modes true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

      target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
          ${CMAKE_CURRENT_LIST_DIR}/examples/mcuxClRandomModes/mcuxClRandomModes_CtrDrbg_AES256_DRG3_example.c
          ${CMAKE_CURRENT_LIST_DIR}/examples/mcuxClRandomModes/mcuxClRandomModes_CtrDrbg_AES256_DRG4_example.c
          ${CMAKE_CURRENT_LIST_DIR}/examples/mcuxClRandomModes/mcuxClRandomModes_CtrDrbg_AES256_ELS_example.c
          ${CMAKE_CURRENT_LIST_DIR}/examples/mcuxClRandomModes/mcuxClRandomModes_Different_Sessions_example.c
          ${CMAKE_CURRENT_LIST_DIR}/examples/mcuxClRandomModes/mcuxClRandomModes_PatchMode_CtrDrbg_AES256_DRG3_example.c
          ${CMAKE_CURRENT_LIST_DIR}/examples/mcuxClRandomModes/mcuxClRandomModes_TestMode_CtrDrbg_AES256_DRG4_example.c
        )

  
      target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
          ${CMAKE_CURRENT_LIST_DIR}/.
          ${CMAKE_CURRENT_LIST_DIR}/examples/mcuxCsslFlowProtection/inc
          ${CMAKE_CURRENT_LIST_DIR}/examples/mcuxCsslMemory/inc
          ${CMAKE_CURRENT_LIST_DIR}/src/comps/mcuxClExample/inc
          ${CMAKE_CURRENT_LIST_DIR}/src/comps/mcuxClBuffer/inc
        )

  
