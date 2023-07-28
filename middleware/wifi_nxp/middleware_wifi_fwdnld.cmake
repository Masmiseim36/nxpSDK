# Add set(CONFIG_USE_middleware_wifi_fwdnld true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

if(CONFIG_USE_middleware_freertos-kernel AND CONFIG_USE_middleware_wifi_template AND CONFIG_USE_middleware_wifi_mlan_sdio AND CONFIG_USE_middleware_wifi_common_files)

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}/wifidriver/sdio.c
  ${CMAKE_CURRENT_LIST_DIR}/wifidriver/firmware_dnld.c
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/incl
  ${CMAKE_CURRENT_LIST_DIR}/wifi_bt_firmware
  ${CMAKE_CURRENT_LIST_DIR}/wifidriver
  ${CMAKE_CURRENT_LIST_DIR}/wifidriver/incl
)

else()

message(SEND_ERROR "middleware_wifi_fwdnld dependency does not meet, please check ${CMAKE_CURRENT_LIST_FILE}.")

endif()
