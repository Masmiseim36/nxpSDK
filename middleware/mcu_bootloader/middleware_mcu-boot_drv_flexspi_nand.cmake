# Add set(CONFIG_USE_middleware_mcu-boot_drv_flexspi_nand true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

if((CONFIG_DEVICE_ID STREQUAL MIMXRT1052xxxxB OR CONFIG_DEVICE_ID STREQUAL MIMXRT1021xxxxx OR CONFIG_DEVICE_ID STREQUAL MIMXRT1024xxxxx OR CONFIG_DEVICE_ID STREQUAL MIMXRT1062xxxxA OR CONFIG_DEVICE_ID STREQUAL MIMXRT1062xxxxB OR CONFIG_DEVICE_ID STREQUAL MIMXRT1064xxxxA OR CONFIG_DEVICE_ID STREQUAL MIMXRT1015xxxxx OR CONFIG_DEVICE_ID STREQUAL MIMXRT1011xxxxx OR CONFIG_DEVICE_ID STREQUAL MIMXRT1176xxxxx OR CONFIG_DEVICE_ID STREQUAL MIMXRT1166xxxxx OR CONFIG_DEVICE_ID STREQUAL MIMXRT1042xxxxB))

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}/src/drivers/flexspi/bl_flexspi.c
  ${CMAKE_CURRENT_LIST_DIR}/src/drivers/flexspi_nand/flexspi_nand_flash.c
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/src/drivers/flexspi
  ${CMAKE_CURRENT_LIST_DIR}/src/drivers/flexspi_nand
)

else()

message(SEND_ERROR "middleware_mcu-boot_drv_flexspi_nand dependency does not meet, please check ${CMAKE_CURRENT_LIST_FILE}.")

endif()
