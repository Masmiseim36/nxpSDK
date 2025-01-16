# Add set(CONFIG_USE_middleware_wifi_fwdnld true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

      target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
          ${CMAKE_CURRENT_LIST_DIR}/wifi_bt_firmware/8801/8801_cpu1.c
          ${CMAKE_CURRENT_LIST_DIR}/wifi_bt_firmware/IW416/IW416_cpu12.c
          ${CMAKE_CURRENT_LIST_DIR}/wifi_bt_firmware/IW416/IW416_cpu1.c
          ${CMAKE_CURRENT_LIST_DIR}/wifi_bt_firmware/IW416/IW416_cpu2.c
          ${CMAKE_CURRENT_LIST_DIR}/wifi_bt_firmware/8987/8987_cpu12.c
          ${CMAKE_CURRENT_LIST_DIR}/wifi_bt_firmware/8987/8987_cpu1.c
          ${CMAKE_CURRENT_LIST_DIR}/wifi_bt_firmware/8987/8987_cpu2.c
          ${CMAKE_CURRENT_LIST_DIR}/wifi_bt_firmware/nw61x/nw61x_cpu12_se.c
          ${CMAKE_CURRENT_LIST_DIR}/wifi_bt_firmware/nw61x/nw61x_cpu1_se.c
          ${CMAKE_CURRENT_LIST_DIR}/wifi_bt_firmware/nw61x/nw61x_cpu2_se.c
          ${CMAKE_CURRENT_LIST_DIR}/wifi_bt_firmware/iw610/iw610_cpu12_se.c
          ${CMAKE_CURRENT_LIST_DIR}/wifi_bt_firmware/iw610/iw610_cpu2_se.c
          ${CMAKE_CURRENT_LIST_DIR}/wifi_bt_firmware/iw610/iw610_cpu1_se.c
          ${CMAKE_CURRENT_LIST_DIR}/wifidriver/sdio.c
          ${CMAKE_CURRENT_LIST_DIR}/firmware_dnld/firmware_dnld.c
        )

  
      target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
          ${CMAKE_CURRENT_LIST_DIR}/incl
          ${CMAKE_CURRENT_LIST_DIR}/wifi_bt_firmware
          ${CMAKE_CURRENT_LIST_DIR}/wifi_bt_firmware/8801
          ${CMAKE_CURRENT_LIST_DIR}/wifi_bt_firmware/IW416
          ${CMAKE_CURRENT_LIST_DIR}/wifi_bt_firmware/8987
          ${CMAKE_CURRENT_LIST_DIR}/wifi_bt_firmware/nw61x
          ${CMAKE_CURRENT_LIST_DIR}/wifi_bt_firmware/iw610
          ${CMAKE_CURRENT_LIST_DIR}/wifidriver
          ${CMAKE_CURRENT_LIST_DIR}/wifidriver/incl
          ${CMAKE_CURRENT_LIST_DIR}/firmware_dnld
          ${CMAKE_CURRENT_LIST_DIR}/sdio_nxp_abs
          ${CMAKE_CURRENT_LIST_DIR}/sdio_nxp_abs/incl
          ${CMAKE_CURRENT_LIST_DIR}/fwdnld_intf_abs
        )

  
