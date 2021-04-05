if(NOT DRIVER_CMSIS_IUART_MIMX8MM6_INCLUDED)
    
    set(DRIVER_CMSIS_IUART_MIMX8MM6_INCLUDED true CACHE BOOL "driver_cmsis_iuart component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/fsl_uart_cmsis.c
    )


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/.
    )


    include(driver_iuart_MIMX8MM6)

    include(CMSIS_Driver_Include_USART_MIMX8MM6)

endif()
