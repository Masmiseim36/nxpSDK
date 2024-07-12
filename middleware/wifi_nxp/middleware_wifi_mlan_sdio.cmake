# Add set(CONFIG_USE_middleware_wifi_mlan_sdio true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

if(CONFIG_USE_middleware_wifi_template AND CONFIG_USE_middleware_sdmmc_sdio AND CONFIG_USE_middleware_sdmmc_host_usdhc AND CONFIG_USE_middleware_wifi_common_files AND ((CONFIG_USE_middleware_sdmmc_host_usdhc_freertos) OR (CONFIG_USE_middleware_sdmmc_host_usdhc_azurertos)))

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}/sdio_nxp_abs/mlan_sdio.c
  ${CMAKE_CURRENT_LIST_DIR}/sdio_nxp_abs/fwdnld_sdio.c
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/wifidriver
  ${CMAKE_CURRENT_LIST_DIR}/sdio_nxp_abs/incl
)

else()

message(SEND_ERROR "middleware_wifi_mlan_sdio dependency does not meet, please check ${CMAKE_CURRENT_LIST_FILE}.")

endif()
