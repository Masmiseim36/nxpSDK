if(NOT COMPONENT_SERIAL_MANAGER_UART_MIMXRT117H_INCLUDED)
    
    set(COMPONENT_SERIAL_MANAGER_UART_MIMXRT117H_INCLUDED true CACHE BOOL "component_serial_manager_uart component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/fsl_component_serial_port_uart.c
    )


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/.
    )


    #OR Logic component
    if(CONFIG_USE_driver_lpuart_MIMXRT117H)
         include(driver_lpuart_MIMXRT117H)
    endif()
    if(NOT (CONFIG_USE_driver_lpuart_MIMXRT117H))
        message(WARNING "Since driver_lpuart_MIMXRT117H is not included at first or config in config.cmake file, use driver_uart_MIMXRT117H by default.")
        include(driver_uart_MIMXRT117H)
    endif()

    include(component_lpuart_adapter_MIMXRT117H)

    include(component_serial_manager_MIMXRT117H)

endif()
