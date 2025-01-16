# Add set(CONFIG_USE_component_els_pkc_oscca_sm4 true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

      target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
          ${CMAKE_CURRENT_LIST_DIR}/src/comps/mcuxClOsccaSm4/src/mcuxClOsccaSm4_CommonOperations.c
          ${CMAKE_CURRENT_LIST_DIR}/src/comps/mcuxClOsccaSm4/src/mcuxClOsccaSm4_Core.c
          ${CMAKE_CURRENT_LIST_DIR}/src/comps/mcuxClOsccaSm4/src/mcuxClOsccaSm4_KeyTypes.c
          ${CMAKE_CURRENT_LIST_DIR}/src/comps/mcuxClOsccaSm4/src/mcuxClOsccaSm4_LoadKey.c
        )

  
      target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
          ${CMAKE_CURRENT_LIST_DIR}/.
          ${CMAKE_CURRENT_LIST_DIR}/src/comps/mcuxClOsccaSm4/inc
          ${CMAKE_CURRENT_LIST_DIR}/src/comps/mcuxClOsccaSm4/inc/internal
        )

  
