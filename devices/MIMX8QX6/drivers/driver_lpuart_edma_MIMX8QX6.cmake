if(NOT DRIVER_LPUART_EDMA_MIMX8QX6_INCLUDED)
    
    set(DRIVER_LPUART_EDMA_MIMX8QX6_INCLUDED true CACHE BOOL "driver_lpuart_edma component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/fsl_lpuart_edma.c
    )


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/.
    )


    include(driver_dma3_MIMX8QX6)

    include(driver_lpuart_MIMX8QX6)

endif()
