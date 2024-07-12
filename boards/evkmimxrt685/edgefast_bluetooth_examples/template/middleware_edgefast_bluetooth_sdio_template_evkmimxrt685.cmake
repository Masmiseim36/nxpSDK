# Add set(CONFIG_USE_middleware_edgefast_bluetooth_sdio_template_evkmimxrt685 true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

if(CONFIG_USE_middleware_sdmmc_host_usdhc AND CONFIG_USE_middleware_sdmmc_sdio AND CONFIG_USE_middleware_sdmmc_host_usdhc_freertos AND CONFIG_USE_component_rt_gpio_adapter AND (CONFIG_DEVICE_ID STREQUAL MIMXRT685S) AND (CONFIG_BOARD STREQUAL evkmimxrt685))

else()

message(SEND_ERROR "middleware_edgefast_bluetooth_sdio_template_evkmimxrt685 dependency does not meet, please check ${CMAKE_CURRENT_LIST_FILE}.")

endif()
