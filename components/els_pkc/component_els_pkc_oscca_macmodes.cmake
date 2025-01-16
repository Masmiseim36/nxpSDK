# Add set(CONFIG_USE_component_els_pkc_oscca_macmodes true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

      target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
          ${CMAKE_CURRENT_LIST_DIR}/src/comps/mcuxClOsccaMacModes/src/mcuxClOsccaMacModes.c
          ${CMAKE_CURRENT_LIST_DIR}/src/comps/mcuxClOsccaMacModes/src/mcuxClOsccaMacModes_CBCMac.c
          ${CMAKE_CURRENT_LIST_DIR}/src/comps/mcuxClOsccaMacModes/src/mcuxClOsccaMacModes_CMac.c
          ${CMAKE_CURRENT_LIST_DIR}/src/comps/mcuxClOsccaMacModes/src/mcuxClOsccaMacModes_Helper.c
          ${CMAKE_CURRENT_LIST_DIR}/src/comps/mcuxClOsccaMacModes/src/mcuxClOsccaMacModes_KeyTypes.c
          ${CMAKE_CURRENT_LIST_DIR}/src/comps/mcuxClOsccaMacModes/src/mcuxClOsccaMacModes_Modes.c
        )

  
      target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
          ${CMAKE_CURRENT_LIST_DIR}/.
          ${CMAKE_CURRENT_LIST_DIR}/src/comps/mcuxClOsccaMacModes/inc
          ${CMAKE_CURRENT_LIST_DIR}/src/comps/mcuxClOsccaMacModes/inc/internal
        )

  
