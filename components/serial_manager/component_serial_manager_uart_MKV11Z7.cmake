if(NOT COMPONENT_SERIAL_MANAGER_UART_MKV11Z7_INCLUDED)
    
    set(COMPONENT_SERIAL_MANAGER_UART_MKV11Z7_INCLUDED true CACHE BOOL "component_serial_manager_uart component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/fsl_component_serial_port_uart.c
    )


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/.
    )


    #OR Logic component
    if(CONFIG_USE_driver_uart_MKV11Z7)
         include(driver_uart_MKV11Z7)
    endif()
    if(NOT (CONFIG_USE_driver_uart_MKV11Z7))
        message(WARNING "Since driver_uart_MKV11Z7 is not included at first or config in config.cmake file, use driver_uart_MKV11Z7 by default.")
        include(driver_uart_MKV11Z7)
    endif()

    include(component_uart_adapter_MKV11Z7)

    include(component_serial_manager_MKV11Z7)

endif()
