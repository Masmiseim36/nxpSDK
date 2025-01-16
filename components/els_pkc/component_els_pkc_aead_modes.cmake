# Add set(CONFIG_USE_component_els_pkc_aead_modes true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

      target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
          ${CMAKE_CURRENT_LIST_DIR}/src/comps/mcuxClAeadModes/src/mcuxClAeadModes_Els_AesCcm.c
          ${CMAKE_CURRENT_LIST_DIR}/src/comps/mcuxClAeadModes/src/mcuxClAeadModes_Els_AesCcmEngine.c
          ${CMAKE_CURRENT_LIST_DIR}/src/comps/mcuxClAeadModes/src/mcuxClAeadModes_Els_AesGcm.c
          ${CMAKE_CURRENT_LIST_DIR}/src/comps/mcuxClAeadModes/src/mcuxClAeadModes_Els_AesGcmEngine.c
          ${CMAKE_CURRENT_LIST_DIR}/src/comps/mcuxClAeadModes/src/mcuxClAeadModes_Els_Modes.c
          ${CMAKE_CURRENT_LIST_DIR}/src/comps/mcuxClAeadModes/src/mcuxClAeadModes_Els_Multipart.c
          ${CMAKE_CURRENT_LIST_DIR}/src/comps/mcuxClAeadModes/src/mcuxClAeadModes_Els_Oneshot.c
        )

  
      target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
          ${CMAKE_CURRENT_LIST_DIR}/.
          ${CMAKE_CURRENT_LIST_DIR}/src/comps/mcuxClAeadModes/inc
          ${CMAKE_CURRENT_LIST_DIR}/src/comps/mcuxClAeadModes/inc/internal
        )

  
