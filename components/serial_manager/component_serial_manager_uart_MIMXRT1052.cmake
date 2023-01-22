include_guard()
message("component_serial_manager_uart component is included.")

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/fsl_component_serial_port_uart.c
)


target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/.
)


#OR Logic component
if(CONFIG_USE_driver_lpuart_MIMXRT1052)
     include(driver_lpuart_MIMXRT1052)
endif()
if(NOT (CONFIG_USE_driver_lpuart_MIMXRT1052))
    message(WARNING "Since driver_lpuart_MIMXRT1052 is not included at first or config in config.cmake file, use driver_uart_MIMXRT1052 by default.")
    include(driver_uart_MIMXRT1052)
endif()

include(component_lpuart_adapter_MIMXRT1052)

include(component_serial_manager_MIMXRT1052)

