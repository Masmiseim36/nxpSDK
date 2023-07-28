# Add set(CONFIG_USE_middleware_mcu-boot_MIMXRT1011_drivers true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

if((CONFIG_DEVICE_ID STREQUAL MIMXRT1011xxxxx))

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}/../../platform/drivers/igpio/fsl_gpio.c
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/../../platform/drivers/common
  ${CMAKE_CURRENT_LIST_DIR}/../../devices/MIMXRT1011/drivers
  ${CMAKE_CURRENT_LIST_DIR}/../../platform/drivers/wdog01
  ${CMAKE_CURRENT_LIST_DIR}/../../platform/drivers/rtwdog
  ${CMAKE_CURRENT_LIST_DIR}/../../platform/drivers/igpio
  ${CMAKE_CURRENT_LIST_DIR}/../../platform/drivers/lpuart
)

else()

message(SEND_ERROR "middleware_mcu-boot_MIMXRT1011_drivers dependency does not meet, please check ${CMAKE_CURRENT_LIST_FILE}.")

endif()
