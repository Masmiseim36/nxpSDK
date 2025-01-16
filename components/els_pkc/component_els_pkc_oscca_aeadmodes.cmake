# Add set(CONFIG_USE_component_els_pkc_oscca_aeadmodes true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

      target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
          ${CMAKE_CURRENT_LIST_DIR}/src/comps/mcuxClOsccaAeadModes/src/mcuxClOsccaAeadModes_Ccm_Common.c
          ${CMAKE_CURRENT_LIST_DIR}/src/comps/mcuxClOsccaAeadModes/src/mcuxClOsccaAeadModes_Constants.c
          ${CMAKE_CURRENT_LIST_DIR}/src/comps/mcuxClOsccaAeadModes/src/mcuxClOsccaAeadModes_EngineCcm.c
          ${CMAKE_CURRENT_LIST_DIR}/src/comps/mcuxClOsccaAeadModes/src/mcuxClOsccaAeadModes_SkeletonCcm.c
          ${CMAKE_CURRENT_LIST_DIR}/src/comps/mcuxClOsccaAeadModes/src/mcuxClOsccaAeadModes_SM4Ctr.c
          ${CMAKE_CURRENT_LIST_DIR}/src/comps/mcuxClOsccaAeadModes/src/mcuxClOsccaAeadModes_SM4_Multipart.c
          ${CMAKE_CURRENT_LIST_DIR}/src/comps/mcuxClOsccaAeadModes/src/mcuxClOsccaAeadModes_SM4_OneShot.c
        )

  
      target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
          ${CMAKE_CURRENT_LIST_DIR}/.
          ${CMAKE_CURRENT_LIST_DIR}/src/comps/mcuxClOsccaAeadModes/inc
          ${CMAKE_CURRENT_LIST_DIR}/src/comps/mcuxClOsccaAeadModes/inc/internal
        )

  
