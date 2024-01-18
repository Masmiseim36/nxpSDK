# Add set(CONFIG_USE_middleware_mcu-boot_bootsources_imxrt true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

if((CONFIG_DEVICE_ID STREQUAL MIMXRT1052xxxxB OR CONFIG_DEVICE_ID STREQUAL MIMXRT1021xxxxx OR CONFIG_DEVICE_ID STREQUAL MIMXRT1024xxxxx OR CONFIG_DEVICE_ID STREQUAL MIMXRT1062xxxxA OR CONFIG_DEVICE_ID STREQUAL MIMXRT1062xxxxB OR CONFIG_DEVICE_ID STREQUAL MIMXRT1064xxxxA OR CONFIG_DEVICE_ID STREQUAL MIMXRT1064xxxxB OR CONFIG_DEVICE_ID STREQUAL MIMXRT1015xxxxx OR CONFIG_DEVICE_ID STREQUAL MIMXRT1176xxxxx OR CONFIG_DEVICE_ID STREQUAL MIMXRT1166xxxxx OR CONFIG_DEVICE_ID STREQUAL MIMXRT1042xxxxB))

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}/src/drivers/dcp/bl_dcp.c
  ${CMAKE_CURRENT_LIST_DIR}/src/drivers/trng/bl_trng.c
  ${CMAKE_CURRENT_LIST_DIR}/src/bootloader/src/lpuart_peripheral_interface.c
  ${CMAKE_CURRENT_LIST_DIR}/src/bootloader/src/usb_hid_msc_peripheral_interface.c
  ${CMAKE_CURRENT_LIST_DIR}/src/bootloader/src/bl_nor_encrypt_bee.c
  ${CMAKE_CURRENT_LIST_DIR}/src/bootloader/src/bl_keyblob.c
  ${CMAKE_CURRENT_LIST_DIR}/src/bootloader/src/bl_keyblob_dcp.c
  ${CMAKE_CURRENT_LIST_DIR}/src/property/src/property_imx.c
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/src/drivers/smc
  ${CMAKE_CURRENT_LIST_DIR}/src/drivers/dcp
  ${CMAKE_CURRENT_LIST_DIR}/src/drivers/trng
  ${CMAKE_CURRENT_LIST_DIR}/src/bootloader
)

else()

message(SEND_ERROR "middleware_mcu-boot_bootsources_imxrt dependency does not meet, please check ${CMAKE_CURRENT_LIST_FILE}.")

endif()
