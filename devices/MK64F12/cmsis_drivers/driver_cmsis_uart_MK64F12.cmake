if(NOT DRIVER_CMSIS_UART_MK64F12_INCLUDED)
    
    set(DRIVER_CMSIS_UART_MK64F12_INCLUDED true CACHE BOOL "driver_cmsis_uart component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/fsl_uart_cmsis.c
    )


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/.
    )


    include(driver_uart_MK64F12)

    include(CMSIS_Driver_Include_USART_MK64F12)

endif()
