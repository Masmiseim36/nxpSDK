if(NOT DRIVER_CMSIS_DSPI_MK66F18_INCLUDED)
    
    set(DRIVER_CMSIS_DSPI_MK66F18_INCLUDED true CACHE BOOL "driver_cmsis_dspi component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/fsl_dspi_cmsis.c
    )


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/.
    )


    include(driver_dspi_edma_MK66F18)

    include(CMSIS_Driver_Include_SPI_MK66F18)

endif()
