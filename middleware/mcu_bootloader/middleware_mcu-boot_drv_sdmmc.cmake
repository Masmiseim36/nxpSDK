# Add set(CONFIG_USE_middleware_mcu-boot_drv_sdmmc true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

if((CONFIG_DEVICE_ID STREQUAL MIMXRT1052xxxxB OR CONFIG_DEVICE_ID STREQUAL MIMXRT1021xxxxx OR CONFIG_DEVICE_ID STREQUAL MIMXRT1062xxxxA OR CONFIG_DEVICE_ID STREQUAL MIMXRT1062xxxxB OR CONFIG_DEVICE_ID STREQUAL MIMXRT1064xxxxA OR CONFIG_DEVICE_ID STREQUAL MIMXRT1064xxxxB OR CONFIG_DEVICE_ID STREQUAL MIMXRT1176xxxxx OR CONFIG_DEVICE_ID STREQUAL MIMXRT1166xxxxx OR CONFIG_DEVICE_ID STREQUAL MIMXRT1042xxxxB))

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}/src/drivers/usdhc/bl_usdhc.c
  ${CMAKE_CURRENT_LIST_DIR}/src/drivers/sdmmc/src/fsl_mmc.c
  ${CMAKE_CURRENT_LIST_DIR}/src/drivers/sdmmc/src/fsl_sd.c
  ${CMAKE_CURRENT_LIST_DIR}/src/drivers/sdmmc/src/fsl_sdmmc_common.c
  ${CMAKE_CURRENT_LIST_DIR}/src/drivers/sdmmc/port/usdhc/polling/fsl_sdmmc_host.c
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/src/drivers/usdhc
  ${CMAKE_CURRENT_LIST_DIR}/src/drivers/sdmmc/inc
)

else()

message(SEND_ERROR "middleware_mcu-boot_drv_sdmmc dependency does not meet, please check ${CMAKE_CURRENT_LIST_FILE}.")

endif()
