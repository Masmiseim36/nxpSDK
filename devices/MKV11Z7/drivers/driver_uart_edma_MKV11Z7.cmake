if(NOT DRIVER_UART_EDMA_MKV11Z7_INCLUDED)
    
    set(DRIVER_UART_EDMA_MKV11Z7_INCLUDED true CACHE BOOL "driver_uart_edma component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/fsl_uart_edma.c
    )


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/.
    )


    include(driver_uart_MKV11Z7)

    include(driver_edma_MKV11Z7)

endif()
