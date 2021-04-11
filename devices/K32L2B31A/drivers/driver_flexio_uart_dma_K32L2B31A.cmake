if(NOT DRIVER_FLEXIO_UART_DMA_K32L2B31A_INCLUDED)
    
    set(DRIVER_FLEXIO_UART_DMA_K32L2B31A_INCLUDED true CACHE BOOL "driver_flexio_uart_dma component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/fsl_flexio_uart_dma.c
    )


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/.
    )


    include(driver_dma_K32L2B31A)

    include(driver_flexio_uart_K32L2B31A)

endif()
