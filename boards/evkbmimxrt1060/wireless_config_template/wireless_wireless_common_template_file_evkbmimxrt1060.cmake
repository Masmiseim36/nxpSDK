# Add set(CONFIG_USE_wireless_wireless_common_template_file_evkbmimxrt1060 true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

if((CONFIG_BOARD STREQUAL evkbmimxrt1060))

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}/wifi_bt_config.c
  ${CMAKE_CURRENT_LIST_DIR}/sdmmc_config.c
)

else()

message(SEND_ERROR "wireless_wireless_common_template_file_evkbmimxrt1060 dependency does not meet, please check ${CMAKE_CURRENT_LIST_FILE}.")

endif()
