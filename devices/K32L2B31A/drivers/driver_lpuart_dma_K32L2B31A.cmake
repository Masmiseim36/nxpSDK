if(NOT DRIVER_LPUART_DMA_K32L2B31A_INCLUDED)
    
    set(DRIVER_LPUART_DMA_K32L2B31A_INCLUDED true CACHE BOOL "driver_lpuart_dma component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/fsl_lpuart_dma.c
    )


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/.
    )


    include(driver_lpuart_K32L2B31A)

    include(driver_dma_K32L2B31A)

endif()
