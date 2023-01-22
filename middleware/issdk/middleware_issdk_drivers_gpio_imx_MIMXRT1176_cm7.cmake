include_guard()
message("middleware_issdk_drivers_gpio_imx component is included.")

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/drivers/gpio/i.mx/gpio_driver_irq.c
    ${CMAKE_CURRENT_LIST_DIR}/drivers/gpio/i.mx/gpio_driver.c
)


target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/drivers/gpio/i.mx
    ${CMAKE_CURRENT_LIST_DIR}/drivers/gpio
)


include(middleware_issdk_drivers_cmsis_drivers_MIMXRT1176_cm7)

