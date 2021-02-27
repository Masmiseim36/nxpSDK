if(NOT DRIVER_FLEXCOMM_SPI_DMA_MIMXRT595S_cm33_INCLUDED)
    
    set(DRIVER_FLEXCOMM_SPI_DMA_MIMXRT595S_cm33_INCLUDED true CACHE BOOL "driver_flexcomm_spi_dma component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/fsl_spi_dma.c
    )


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/.
    )


    include(driver_flexcomm_MIMXRT595S_cm33)

    include(driver_flexcomm_spi_MIMXRT595S_cm33)

    include(driver_lpc_dma_MIMXRT595S_cm33)

endif()
