if(NOT DRIVER_FLEXIO_SPI_EDMA_MIMXRT1062_INCLUDED)
    
    set(DRIVER_FLEXIO_SPI_EDMA_MIMXRT1062_INCLUDED true CACHE BOOL "driver_flexio_spi_edma component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/fsl_flexio_spi_edma.c
    )


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/.
    )


    include(driver_edma_MIMXRT1062)

    include(driver_flexio_spi_MIMXRT1062)

endif()
