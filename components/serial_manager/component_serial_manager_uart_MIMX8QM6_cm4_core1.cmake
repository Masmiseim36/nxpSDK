if(NOT COMPONENT_SERIAL_MANAGER_UART_MIMX8QM6_cm4_core1_INCLUDED)
    
    set(COMPONENT_SERIAL_MANAGER_UART_MIMX8QM6_cm4_core1_INCLUDED true CACHE BOOL "component_serial_manager_uart component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/fsl_component_serial_port_uart.c
    )


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/.
    )


    #OR Logic component
    if(CONFIG_USE_driver_lpuart_MIMX8QM6_cm4_core1)
         include(driver_lpuart_MIMX8QM6_cm4_core1)
    endif()
    if(NOT (CONFIG_USE_driver_lpuart_MIMX8QM6_cm4_core1))
        message(WARNING "Since driver_lpuart_MIMX8QM6_cm4_core1 is not included at first or config in config.cmake file, use driver_uart_MIMX8QM6_cm4_core1 by default.")
        include(driver_uart_MIMX8QM6_cm4_core1)
    endif()

    include(component_lpuart_adapter_MIMX8QM6_cm4_core1)

    include(component_serial_manager_MIMX8QM6_cm4_core1)

endif()
