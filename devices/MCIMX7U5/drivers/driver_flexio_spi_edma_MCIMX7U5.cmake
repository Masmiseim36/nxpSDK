if(NOT DRIVER_FLEXIO_SPI_EDMA_MCIMX7U5_INCLUDED)
    
    set(DRIVER_FLEXIO_SPI_EDMA_MCIMX7U5_INCLUDED true CACHE BOOL "driver_flexio_spi_edma component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/fsl_flexio_spi_edma.c
    )


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/.
    )


    include(driver_edma_MCIMX7U5)

    include(driver_flexio_spi_MCIMX7U5)

endif()
