# Add set(CONFIG_USE_component_els_pkc_trng_type_els true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

      target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
          ${CMAKE_CURRENT_LIST_DIR}/src/comps/mcuxClRandomModes/src/mcuxClRandomModes_NormalMode.c
          ${CMAKE_CURRENT_LIST_DIR}/src/comps/mcuxClTrng/src/mcuxClTrng_ELS.c
        )

  
      target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
          ${CMAKE_CURRENT_LIST_DIR}/.
        )

  
