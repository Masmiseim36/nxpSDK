include_guard()
message("component_serial_mwm_lpuart component is included.")

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/serial_mwm_lpuart.c
)


include(driver_lpuart_freertos_MIMXRT1011)

