# Add set(CONFIG_USE_middleware_wifi_sdio true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

if(CONFIG_USE_middleware_wifi_mlan_sdio AND CONFIG_USE_middleware_wifi_wifidriver)

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}/wifidriver/wifi-sdio.c
)

else()

message(SEND_ERROR "middleware_wifi_sdio dependency does not meet, please check ${CMAKE_CURRENT_LIST_FILE}.")

endif()
