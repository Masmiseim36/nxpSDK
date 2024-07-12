# Add set(CONFIG_USE_middleware_edgefast_bluetooth_wifi_nxp_controller_base true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

if(CONFIG_USE_middleware_edgefast_bluetooth_common_ethermind AND CONFIG_USE_middleware_wifi_fwdnld AND ((CONFIG_USE_middleware_edgefast_bluetooth_sdio_template_evkmimxrt1040 AND (CONFIG_BOARD STREQUAL evkmimxrt1040)) OR (CONFIG_USE_middleware_edgefast_bluetooth_sdio_template_evkcmimxrt1060 AND (CONFIG_BOARD STREQUAL evkcmimxrt1060)) OR (CONFIG_USE_middleware_edgefast_bluetooth_sdio_template_evkmimxrt685 AND (CONFIG_BOARD STREQUAL evkmimxrt685)) OR (CONFIG_USE_middleware_edgefast_bluetooth_sdio_template_mimxrt685audevk AND (CONFIG_BOARD STREQUAL mimxrt685audevk)) OR (CONFIG_USE_middleware_edgefast_bluetooth_sdio_template_evkmimxrt595 AND (CONFIG_BOARD STREQUAL evkmimxrt595)) OR (CONFIG_USE_middleware_edgefast_bluetooth_sdio_template_evkbimxrt1050 AND (CONFIG_BOARD STREQUAL evkbimxrt1050)) OR (CONFIG_USE_middleware_edgefast_bluetooth_sdio_template_evkbmimxrt1170 AND (CONFIG_BOARD STREQUAL evkbmimxrt1170)) OR (CONFIG_USE_middleware_edgefast_bluetooth_sdio_template_evkmimxrt1180 AND (CONFIG_BOARD STREQUAL evkmimxrt1180)) OR (CONFIG_USE_middleware_edgefast_bluetooth_sdio_template_mcxn5xxevk AND (CONFIG_BOARD STREQUAL mcxn5xxevk)) OR (CONFIG_USE_middleware_edgefast_bluetooth_sdio_template_mcxn9xxevk AND (CONFIG_BOARD STREQUAL mcxn9xxevk))))

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}/port/pal/mcux/bluetooth/controller/controller_wifi_nxp.c
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/port/pal/mcux/bluetooth/controller
)

else()

message(SEND_ERROR "middleware_edgefast_bluetooth_wifi_nxp_controller_base dependency does not meet, please check ${CMAKE_CURRENT_LIST_FILE}.")

endif()
