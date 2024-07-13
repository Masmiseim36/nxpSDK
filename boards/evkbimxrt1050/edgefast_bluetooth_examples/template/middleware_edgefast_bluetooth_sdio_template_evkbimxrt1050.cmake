# Add set(CONFIG_USE_middleware_edgefast_bluetooth_sdio_template_evkbimxrt1050 true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

if(CONFIG_USE_middleware_sdmmc_host_usdhc AND CONFIG_USE_middleware_sdmmc_sdio AND CONFIG_USE_middleware_sdmmc_host_usdhc_freertos AND CONFIG_USE_component_igpio_adapter AND (CONFIG_DEVICE_ID STREQUAL MIMXRT1052xxxxB) AND (CONFIG_BOARD STREQUAL evkbimxrt1050))

else()

message(SEND_ERROR "middleware_edgefast_bluetooth_sdio_template_evkbimxrt1050 dependency does not meet, please check ${CMAKE_CURRENT_LIST_FILE}.")

endif()
