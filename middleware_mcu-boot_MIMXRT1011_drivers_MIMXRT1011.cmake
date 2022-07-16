include_guard()
message("middleware_mcu-boot_MIMXRT1011_drivers component is included.")

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/platform/drivers/igpio/fsl_gpio.c
)


target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/platform/drivers/common
    ${CMAKE_CURRENT_LIST_DIR}/devices/MIMXRT1011/drivers
    ${CMAKE_CURRENT_LIST_DIR}/platform/drivers/wdog01
    ${CMAKE_CURRENT_LIST_DIR}/platform/drivers/rtwdog
    ${CMAKE_CURRENT_LIST_DIR}/platform/drivers/igpio
    ${CMAKE_CURRENT_LIST_DIR}/platform/drivers/lpuart
)


