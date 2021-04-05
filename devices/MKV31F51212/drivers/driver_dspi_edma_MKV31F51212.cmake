if(NOT DRIVER_DSPI_EDMA_MKV31F51212_INCLUDED)
    
    set(DRIVER_DSPI_EDMA_MKV31F51212_INCLUDED true CACHE BOOL "driver_dspi_edma component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/fsl_dspi_edma.c
    )


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/.
    )


    include(driver_dspi_MKV31F51212)

    include(driver_edma_MKV31F51212)

endif()
