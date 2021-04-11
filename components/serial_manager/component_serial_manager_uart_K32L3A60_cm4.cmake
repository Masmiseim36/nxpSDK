if(NOT COMPONENT_SERIAL_MANAGER_UART_K32L3A60_cm4_INCLUDED)
    
    set(COMPONENT_SERIAL_MANAGER_UART_K32L3A60_cm4_INCLUDED true CACHE BOOL "component_serial_manager_uart component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/fsl_component_serial_port_uart.c
    )


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/.
    )


    #OR Logic component
    if(CONFIG_USE_driver_lpuart_K32L3A60_cm4)
         include(driver_lpuart_K32L3A60_cm4)
    endif()
    if(NOT (CONFIG_USE_driver_lpuart_K32L3A60_cm4))
        message(WARNING "Since driver_lpuart_K32L3A60_cm4 is not included at first or config in config.cmake file, use driver_uart_K32L3A60_cm4 by default.")
        include(driver_uart_K32L3A60_cm4)
    endif()

    include(component_lpuart_adapter_K32L3A60_cm4)

    include(component_serial_manager_K32L3A60_cm4)

endif()
