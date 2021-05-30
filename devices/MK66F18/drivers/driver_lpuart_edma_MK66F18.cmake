if(NOT DRIVER_LPUART_EDMA_MK66F18_INCLUDED)
    
    set(DRIVER_LPUART_EDMA_MK66F18_INCLUDED true CACHE BOOL "driver_lpuart_edma component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/fsl_lpuart_edma.c
    )


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/.
    )


    include(driver_edma_MK66F18)

    include(driver_lpuart_MK66F18)

endif()
