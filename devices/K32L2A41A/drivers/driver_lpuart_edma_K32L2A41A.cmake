if(NOT DRIVER_LPUART_EDMA_K32L2A41A_INCLUDED)
    
    set(DRIVER_LPUART_EDMA_K32L2A41A_INCLUDED true CACHE BOOL "driver_lpuart_edma component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/fsl_lpuart_edma.c
    )


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/.
    )


    include(driver_edma_K32L2A41A)

    include(driver_lpuart_K32L2A41A)

endif()
