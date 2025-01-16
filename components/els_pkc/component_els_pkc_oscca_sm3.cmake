# Add set(CONFIG_USE_component_els_pkc_oscca_sm3 true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

      target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
          ${CMAKE_CURRENT_LIST_DIR}/src/comps/mcuxClOsccaSm3/src/mcuxClOsccaSm3_core_sm3.c
          ${CMAKE_CURRENT_LIST_DIR}/src/comps/mcuxClOsccaSm3/src/mcuxClOsccaSm3_internal_sm3.c
        )

  
      target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
          ${CMAKE_CURRENT_LIST_DIR}/.
          ${CMAKE_CURRENT_LIST_DIR}/src/comps/mcuxClOsccaSm3/inc
          ${CMAKE_CURRENT_LIST_DIR}/src/comps/mcuxClOsccaSm3/inc/internal
        )

  
