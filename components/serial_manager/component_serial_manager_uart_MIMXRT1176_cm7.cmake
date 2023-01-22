include_guard()
message("component_serial_manager_uart component is included.")

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/fsl_component_serial_port_uart.c
)


target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/.
)


#OR Logic component
if(CONFIG_USE_driver_lpuart_MIMXRT1176_cm7)
     include(driver_lpuart_MIMXRT1176_cm7)
endif()
if(NOT (CONFIG_USE_driver_lpuart_MIMXRT1176_cm7))
    message(WARNING "Since driver_lpuart_MIMXRT1176_cm7 is not included at first or config in config.cmake file, use driver_uart_MIMXRT1176_cm7 by default.")
    include(driver_uart_MIMXRT1176_cm7)
endif()

include(component_lpuart_adapter_MIMXRT1176_cm7)

include(component_serial_manager_MIMXRT1176_cm7)

