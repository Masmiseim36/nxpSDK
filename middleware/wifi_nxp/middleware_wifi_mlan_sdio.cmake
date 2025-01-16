# Add set(CONFIG_USE_middleware_wifi_mlan_sdio true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

      target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
          ${CMAKE_CURRENT_LIST_DIR}/sdio_nxp_abs/mlan_sdio.c
          ${CMAKE_CURRENT_LIST_DIR}/sdio_nxp_abs/fwdnld_sdio.c
        )

  
      target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
          ${CMAKE_CURRENT_LIST_DIR}/wifidriver
          ${CMAKE_CURRENT_LIST_DIR}/sdio_nxp_abs/incl
        )

  
