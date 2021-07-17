include_guard(GLOBAL)
message("middleware_issdk_drivers_gpio_imxrt600 component is included.")

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/drivers/gpio/i.mx/i.mxrt600/gpio_driver.c
)


target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/drivers/gpio/i.mx/i.mxrt600
    ${CMAKE_CURRENT_LIST_DIR}/drivers/gpio
)


