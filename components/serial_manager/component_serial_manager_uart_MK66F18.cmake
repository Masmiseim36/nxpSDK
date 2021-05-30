if(NOT COMPONENT_SERIAL_MANAGER_UART_MK66F18_INCLUDED)
    
    set(COMPONENT_SERIAL_MANAGER_UART_MK66F18_INCLUDED true CACHE BOOL "component_serial_manager_uart component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/fsl_component_serial_port_uart.c
    )


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/.
    )


    #OR Logic component
    if(CONFIG_USE_driver_lpuart_MK66F18)
         include(driver_lpuart_MK66F18)
    endif()
    if(CONFIG_USE_driver_uart_MK66F18)
         include(driver_uart_MK66F18)
    endif()
    if(NOT (CONFIG_USE_driver_lpuart_MK66F18 OR CONFIG_USE_driver_uart_MK66F18))
        message(WARNING "Since driver_lpuart_MK66F18/driver_uart_MK66F18 is not included at first or config in config.cmake file, use driver_uart_MK66F18 by default.")
        include(driver_uart_MK66F18)
    endif()

    include(component_uart_adapter_MK66F18)

    include(component_serial_manager_MK66F18)

endif()
