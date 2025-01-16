# Add set(CONFIG_USE_middleware_wifi_firmware_download true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

      target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
          ${CMAKE_CURRENT_LIST_DIR}/wifidriver/sdio.c
          ${CMAKE_CURRENT_LIST_DIR}/firmware_dnld/firmware_dnld.c
        )

  
      target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
          ${CMAKE_CURRENT_LIST_DIR}/incl
          ${CMAKE_CURRENT_LIST_DIR}/wifidriver
          ${CMAKE_CURRENT_LIST_DIR}/wifidriver/incl
          ${CMAKE_CURRENT_LIST_DIR}/firmware_dnld
          ${CMAKE_CURRENT_LIST_DIR}/sdio_nxp_abs
          ${CMAKE_CURRENT_LIST_DIR}/sdio_nxp_abs/incl
          ${CMAKE_CURRENT_LIST_DIR}/fwdnld_intf_abs
        )

  
