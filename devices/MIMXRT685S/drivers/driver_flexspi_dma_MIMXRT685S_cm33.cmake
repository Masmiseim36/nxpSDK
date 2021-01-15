if(NOT DRIVER_FLEXSPI_DMA_MIMXRT685S_cm33_INCLUDED)
    
    set(DRIVER_FLEXSPI_DMA_MIMXRT685S_cm33_INCLUDED true CACHE BOOL "driver_flexspi_dma component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/fsl_flexspi_dma.c
    )


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/.
    )


    include(driver_flexspi_MIMXRT685S_cm33)

    include(driver_lpc_dma_MIMXRT685S_cm33)

endif()
