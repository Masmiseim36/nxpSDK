# Add set(CONFIG_USE_middleware_sdmmc_host_usdhc true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

if(CONFIG_USE_middleware_sdmmc_host_usdhc_freertos OR CONFIG_USE_middleware_sdmmc_host_usdhc_interrupt OR CONFIG_USE_middleware_sdmmc_host_usdhc_polling OR CONFIG_USE_middleware_sdmmc_host_usdhc_azurertos)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/host/usdhc
)

else()

message(SEND_ERROR "middleware_sdmmc_host_usdhc dependency does not meet, please check ${CMAKE_CURRENT_LIST_FILE}.")

endif()
