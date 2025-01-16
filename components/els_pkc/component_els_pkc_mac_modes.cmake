# Add set(CONFIG_USE_component_els_pkc_mac_modes true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

      target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
          ${CMAKE_CURRENT_LIST_DIR}/src/comps/mcuxClMacModes/src/mcuxClMacModes_Common.c
          ${CMAKE_CURRENT_LIST_DIR}/src/comps/mcuxClMacModes/src/mcuxClMacModes_Common_Modes.c
          ${CMAKE_CURRENT_LIST_DIR}/src/comps/mcuxClMacModes/src/mcuxClMacModes_Els_Cbcmac.c
          ${CMAKE_CURRENT_LIST_DIR}/src/comps/mcuxClMacModes/src/mcuxClMacModes_Els_Cmac.c
          ${CMAKE_CURRENT_LIST_DIR}/src/comps/mcuxClMacModes/src/mcuxClMacModes_Els_Functions.c
        )

  
      target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
          ${CMAKE_CURRENT_LIST_DIR}/.
          ${CMAKE_CURRENT_LIST_DIR}/src/comps/mcuxClMacModes/inc
          ${CMAKE_CURRENT_LIST_DIR}/src/comps/mcuxClMacModes/inc/internal
        )

  
