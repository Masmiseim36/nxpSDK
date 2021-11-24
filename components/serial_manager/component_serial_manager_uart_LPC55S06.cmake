include_guard(GLOBAL)
message("component_serial_manager_uart component is included.")

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/fsl_component_serial_port_uart.c
)


target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/.
)


#OR Logic component
if(CONFIG_USE_driver_flexcomm_usart_LPC55S06)
     include(driver_flexcomm_usart_LPC55S06)
endif()
if(NOT (CONFIG_USE_driver_flexcomm_usart_LPC55S06))
    message(WARNING "Since driver_flexcomm_usart_LPC55S06 is not included at first or config in config.cmake file, use driver_uart_LPC55S06 by default.")
    include(driver_uart_LPC55S06)
endif()

include(component_usart_adapter_LPC55S06)

include(component_serial_manager_LPC55S06)

