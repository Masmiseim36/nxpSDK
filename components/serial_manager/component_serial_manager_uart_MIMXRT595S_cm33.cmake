include_guard()
message("component_serial_manager_uart component is included.")

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/fsl_component_serial_port_uart.c
)


target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/.
)


#OR Logic component
if(CONFIG_USE_driver_flexcomm_usart_MIMXRT595S_cm33)
     include(driver_flexcomm_usart_MIMXRT595S_cm33)
endif()
if(NOT (CONFIG_USE_driver_flexcomm_usart_MIMXRT595S_cm33))
    message(WARNING "Since driver_flexcomm_usart_MIMXRT595S_cm33 is not included at first or config in config.cmake file, use driver_uart_MIMXRT595S_cm33 by default.")
    include(driver_uart_MIMXRT595S_cm33)
endif()

include(component_usart_adapter_MIMXRT595S_cm33)

include(component_serial_manager_MIMXRT595S_cm33)

