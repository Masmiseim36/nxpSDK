# Add set(CONFIG_USE_component_els_pkc_crc true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

      target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
          ${CMAKE_CURRENT_LIST_DIR}/src/comps/mcuxClCrc/src/mcuxClCrc.c
          ${CMAKE_CURRENT_LIST_DIR}/src/comps/mcuxClCrc/src/mcuxClCrc_Sw.c
        )

  
      target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
          ${CMAKE_CURRENT_LIST_DIR}/.
          ${CMAKE_CURRENT_LIST_DIR}/src/comps/mcuxClCrc/inc
          ${CMAKE_CURRENT_LIST_DIR}/src/comps/mcuxClCrc/inc/internal
        )

  
